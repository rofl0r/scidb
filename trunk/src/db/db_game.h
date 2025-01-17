// ======================================================================
// Author : $Author$
// Version: $Revision: 1502 $
// Date   : $Date: 2018-07-16 12:55:14 +0000 (Mon, 16 Jul 2018) $
// Url    : $URL$
// ======================================================================

// ======================================================================
//    _/|            __
//   // o\         /    )           ,        /    /
//   || ._)    ----\---------__----------__-/----/__-
//   //__\          \      /   '  /    /   /    /   )
//   )___(     _(____/____(___ __/____(___/____(___/_
// ======================================================================

// ======================================================================
// Copyright: (C) 2009-2017 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _db_game_included
#define _db_game_included

#include "db_provider.h"
#include "db_game_data.h"
#include "db_board.h"
#include "db_move.h"
#include "db_comment.h"
#include "db_line.h"
#include "db_eco.h"
#include "db_edit_key.h"
#include "db_move_list.h"

#include "u_crc.h"

#include "m_string.h"
#include "m_vector.h"
#include "m_list.h"
#include "m_ref_counted_ptr.h"
#include "m_ref_counter.h"
#include "m_auto_ptr.h"
#include "m_pair.h"

#define DB_DEBUG_GAME

namespace db {

namespace edit { class Root; }
namespace edit { class Node; }

class MoveNode;
class Annotation;
class MarkSet;
class TagSet;

/** @ingroup Core
   The Game class represents a chess game. Moves and variations can be added
	 and removed. Moves can have associated comments and nag values. For methods
	 that accept a variation number 0 is the mainline, with 1 and above being the
	 alternative lines.
*/

class Game : public Provider, private GameData
{
public:

	// Flags indicating how a move string should be constructed
	//
	// These flags may be or-ed together to specify what should be included in
	// a move string.
	enum
	{
		MoveOnly					= 0,			//*< Only the algebraic notation should be included
		WhiteNumbers			= 1 << 0,	//*< White moves should be preceded by a move number
		BlackNumbers			= 1 << 1,	//*< Black moves should be preceded by a move number
		IncludeAnnotation		= 1 << 2,	//*< Nags/symbolic annotation should be included
		SuppressSpace			= 1 << 3,	//*< Don't add space after move number
		UseZeroWidthSpace		= 1 << 4,	//*< Use zero width space after move number
		ExportFormat			= 1 << 5,	//*< Use PGN standard (superseeds other flags)
		LongForm					= 1 << 6,	//*< Print long algebraic notation
		CorrespondenceForm	= 1 << 7,	//*< Print correspondence form
		TelegraphicForm		= 1 << 8,	//*< Print telegraphic form
	};

	enum
	{
		NoUpdate					= 0,
		UpdateBoard				= 1 << 0,
		UpdatePgn				= 1 << 1,
		UpdateOpening			= 1 << 2,
		UpdateLanguageSet		= 1 << 3,
		UpdateIllegalMoves	= 1 << 4,
		UpdateAll				= (1 << 5) - 1,
		UpdateNewPosition		= 1 << 5,
	};

	enum Force
	{
		OnlyIfRemainsConsistent,
		TruncateIfNeccessary,
	};

	enum Command
	{
		None,
		SetAnnotation,
		AddMove,
		AddMoves,
		ExchangeMove,
		NewMainline,
		AddVariation,
		AddVariations,
		ReplaceVariation,
		TruncateVariation,
		FirstVariation,
		PromoteVariation,
		RemoveVariation,
		RemoveVariations,
		InsertMoves,
		ExchangeMoves,
		MergeVariation,
		MergeGame,
		StripMoves,
		StripAnnotations,
		StripComments,
		StripMoveInfo,
		StripMarks,
		StripVariations,
		CopyComments,
		MoveComments,
		Clear,
		Transpose,
	};

	enum Action
	{
		ReplaceNode,
		ReplaceNextNode,
		ExchangeNextNode,
	};

	enum Constraint
	{
		AllowNullMoves,
		DontAllowNullMoves,
	};

	enum ModificationState
	{
		FirstOperation,
		MiddleOperation,
		LastOperation,
	};

	typedef mstl::pair<edit::Key,edit::Key> MergeResult;

	typedef mstl::list<mstl::string> StringList;
	typedef mstl::vector<edit::Node const*> DiffList;
	typedef mstl::vector<Move> History;
	typedef mstl::vector<MoveNode*> Variation;
	typedef mstl::vector<MergeResult> MergeResults;
	typedef Comment::LanguageSet LanguageSet;

	struct Subscriber : public mstl::ref_counter
	{
		virtual ~Subscriber() throw() = 0;

		virtual bool mainlineOnly();

		virtual void stateChanged(bool locked) = 0;

		virtual void updateMarks(mstl::string const& marks) = 0;
		virtual void gotoMove(mstl::string const& key, mstl::string const& succKey) = 0;

		virtual void boardSetup(Board const& board, variant::Type variant) = 0;
		virtual void boardMove(Board const& board, Move const& move, bool forward) = 0;

		virtual void updateOpening() = 0;
		virtual void updateEditor(	edit::Root const* node,
											move::Notation moveStyle) = 0;
		virtual void updateEditor(	DiffList const& nodes,
											TagSet const& tags,
											move::Notation moveStyle,
											termination::State termination,
											color::ID toMove) = 0;
		virtual void updateMergeResults(MergeResults const& mergeResults) = 0;
	};

	typedef mstl::ref_counted_ptr<Subscriber> SubscriberP;
	typedef mstl::auto_ptr<MoveNode> MoveNodeP;

	/// Creates a game with no moves and a standard start position.
	Game();
	/// Copy constructor (the moves will be cloned).
	Game(Game const& game);
	/// Destruct game.
	~Game() throw();

	/// Assign a game.
	Game& operator=(Game const& game);

	/// Accessing game data.
	GameData const& data() const;

	// Querying game information

	/// Return whether the current position is in the mainline
	bool isMainline() const;
	/// Return whether the current position is not in the mainline
	bool isVariation() const;
	/// Return true if the game has been modified
	bool isModified() const;
	/// Return whether game is empty
	bool isEmpty() const;
	/// Return whether the game contains the given language at specified position.
	bool containsLanguage(	edit::Key const& key,
									move::Position position,
									mstl::string const& lang) const;
	/// Return the chess variant of this game.
	variant::Type variant() const override;
	/// Return the start position id.
	uint16_t idn() const override;
	/// Return whether the game is currently at the start position of the mainline
	bool atMainlineStart() const;
	/// Return whether the game is at the end of the mainline
	bool atMainlineEnd() const;
	/// Return whether the game is currently at the start position (of current variation or mainline)
	bool atLineStart() const;
	/// Return whether the game is at the end position (of current variation or mainline)
	bool atLineEnd() const;
	/// Return whether the game is before the end position (of current variation or mainline)
	bool isBeforeLineEnd() const;
	/// Return whether the game is after the start position (of current variation or mainline)
	bool isAfterLineStart() const;
	/// Return whether the game is inside first (not empty) variation
	bool isFirstVariation() const;
	/// Return whether the game is inside last (not empty) variation
	bool isLastVariation() const;
	/// Return whether current node is a part of a game link expansion
	bool isExpansion() const;
	/// Return whether an undo action is possible
	bool hasUndo() const;
	/// Return whether an redo action is possible
	bool hasRedo() const;
	/// Returns whether given variation does not contain invalid moves.
	bool isValidVariation(MoveNode const* node, move::Position position = move::Post) const;
	/// Returns whether given variation does not contain invalid moves.
	bool isValidVariation(MoveList const& moves, move::Position position = move::Post) const;
	/// Return whether given key is valid.
	bool isValidKey(edit::Key const& key) const;
	/// Return whether variation is folded at given position
	bool isFolded(edit::Key const& key) const;
	/// Return whether the game contains variations.
	bool hasVariations() const;
	/// Return whether the game contains move information.
	bool hasMoveInfo(unsigned moveInfoTypes = unsigned(-1)) const;
	/// Return whether the game contains illegal moves (except illegal castlings).
	bool containsIllegalMoves() const;
	/// Return whether the game contains illegal castlings.
	bool containsIllegalCastlings() const;
	/// Returns whether all preceding moves are legal.
	bool historyIsLegal(Constraint constraint) const;

	// Accessing game information

	/// Return game identifier
	unsigned id() const;
	/// Return current position
	Board const& currentBoard() const;
	/// Return position at given key
	Board board(edit::Key const& key) const;
	/// Return position at given key
	Board board(mstl::string const& key) const;
	/// Return start position of game
	Board const& startBoard() const;
	/// Return current move
	Move const& currentMove() const;
	/// Return next move (empty if none available)
	Move const& nextMove() const;
	/// Return next move of given variation (empty if none available)
	Move const& nextMove(unsigned varno) const;
	/// Return side to move
	color::ID sideToMove() const;
	/// Return current move number
	unsigned moveNumber() const;
	/// Return current ply number
	unsigned plyNumber() const;
	/// Return current ply count
	unsigned plyCount() const override;
	/// Return current variation level (mainline is 0)
	unsigned variationLevel() const;
	/// Return current variation number (0 for mainline)
	unsigned variationNumber() const;
	/// Return game flags
	uint32_t gameFlags() const override;
	/// Return the language flags of this game.
	unsigned langFlags() const override;
	/// Return subscriber.
	SubscriberP subscriber() const;
	/// Print current move in given notation
	mstl::string& printMove(mstl::string& result,
									unsigned flags = ExportFormat,
									move::Notation style = move::SAN) const;
	/// Print FEN at current position
	mstl::string& printFen(mstl::string& result) const;
	/// Print FEN at given position
	mstl::string& printFen(mstl::string const& key, mstl::string& result) const;
	/// Return current opening line.
	Line const& openingLine() const override;
	/// Return current ECO code
	Eco const& ecoCode() const;
	/// Return current opening
	Eco const& opening() const;
	/// Return computed ECO code
	Eco computeEcoCode() const;
	/// Fill line to current position, update home pawn data, and current home pawn signature
	uint16_t currentLine(Line& result);
	/// Return comment at current position
	Comment const& comment(move::Position position) const;
	/// Return trailing comment at current position
	Comment const& trailingComment() const;
	/// Return infix annotation at current position
	mstl::string& infix(mstl::string& result) const;
	/// Return prefix annotation at current position
	mstl::string& prefix(mstl::string& result) const;
	/// Return suffix annotation at current position
	mstl::string& suffix(mstl::string& result) const;
	/// Compute checksum of game data.
	util::crc::checksum_t computeChecksum(util::crc::checksum_t crc = 0) const;
	/// Compute checksum of mainline only.
	util::crc::checksum_t computeChecksumOfMainline(util::crc::checksum_t crc = 0) const;
	/// Counts the number of sub-variations at current ply
	unsigned variationCount() const;
	/// Counts the number of sub-variations at next ply
	unsigned subVariationCount() const;
	/// Counts the length (number of half moves) of main line.
	unsigned countLength() const;
	/// Counts the length of line starting at current ply
	unsigned countHalfMoves() const;
	/// Counts the length of line starting at given variation
	unsigned countHalfMoves(unsigned varNo) const;
	/// Counts the length of current line (from start)
	unsigned lengthOfCurrentLine() const;
	/// Counts the number of annotations
	unsigned countAnnotations() const override;
	/// Counts the number of move information
	unsigned countMoveInfo() const override;
	/// Counts the number of move information for specified types
	unsigned countMoveInfo(unsigned moveInfoTypes) const;
	/// Counts the number of marks
	unsigned countMarks() const override;
	/// Counts the number of comments
	unsigned countComments() const override;
	/// Counts the number of variations
	unsigned countVariations() const override;
	/// Get set of tags
	TagSet const& tags() const;
	/// Get marks at current position
	MarkSet const& marks() const;
	/// Get marks at given position
	MarkSet const& marks(edit::Key const& key) const;
	/// Get current language set.
	LanguageSet const& languageSet() const;
	/// Get move style.
	move::Notation moveStyle() const;
	/// Get display flags.
	unsigned displayStyle() const;

	// Moving through game

	/// Moves to the beginning of the game
	void moveToMainlineStart();
	/// Moves to the end of the game
	void moveToMainlineEnd();
	/// Moves to the beginning of current variation (or mainline)
	void moveToStart();
	/// Moves to the end of current variation (or mainline)
	void moveToEnd();
	/// Moves to the given position
	void moveTo(mstl::string const& key);
	/// Moves to the given position
	void moveTo(edit::Key const& key);
	/// Move one move forward, return whether it was possible
	bool forward();
	/// Move one move backward, return whether it was possible
	bool backward();
	/// Move forward the given number of moves, returns actual number of moves made
	unsigned forward(unsigned count);
	/// Move back the given number of moves, returns actual number of moves undone
	unsigned backward(unsigned count);
	/// Enters the variation given by variation number
	void enterVariation(unsigned variationNumber);
	/// Enters the variation given by variation number of next move
	void enterSubVariation(unsigned variationNumber);
	/// Exit current variation, to the parent
	void exitVariation();
	/// Exit variations until mainline is reached
	void exitToMainline();
	/// Return next move.
	mstl::string getNextMove(unsigned flags = ExportFormat);
	/// Get next moves (mainline and sub-variations)
	void getNextMoves(StringList& result, move::Notation form, unsigned flags = ExportFormat) const;
	/// Get current key.
	edit::Key const& currentKey() const;
	/// Get next keys (mainline and sub-variations).
	void getNextKeys(StringList& result) const;
	/// Get key of start position
	mstl::string startKey() const;
	/// Get key of position after current position (empty if at end of main line).
	mstl::string successorKey() const;
	/// Get key after specified position (empty if at end of main line).
	mstl::string nextKey(mstl::string const& key) const;

	// Moving through game using subscriber

	/// Go to start of game.
	void goToMainlineStart();
	/// Go to end of game.
	void goToMainlineEnd();
	/// Go to start of current variation (or main line).
	void goToStart();
	/// Go to end of current variation (or main line).
	void goToEnd();
	/// Go to first move current variation (or main line).
	void goToFirst();
	/// Go to according ply given by key.
	void goTo(mstl::string const& key);
	/// Go to according ply given by key.
	void goTo(edit::Key const& key);
	/// Go forward.
	void goForward(unsigned count = 1);
	/// Go backward.
	void goBackward(unsigned count = 1);
	/// Enter variation after current move.
	void goIntoVariation(unsigned variationNumber);
	/// Leave current variation to successing move.
	void goOutOfVariation();
	/// Enter next variation if any or go one ply forward.
	void goIntoNextVariation();
	/// Enter previous variation if any or go one ply backward.
	void goIntoPrevVariation();
	/// Exit variations until main line is reached.
	void goToMainline();
	/// Go to current key.
	void goToCurrentMove() const;
	/// Go to position given by FEN.
	void goToPosition(mstl::string const& fen);

	// Node modification methods

	/// Sets the comment associated with current move
	void setComment(mstl::string const& comment, move::Position position);
	/// Append the comment associated with current move
	void appendComment(mstl::string const& comment, move::Position position);
	/// Sets the comment associated with current move
	void setTrailingComment(mstl::string const& comment);
	/// Sets the annotation associated with current move
	void setAnnotation(Annotation const& annotation);
	/// Sets the marks associated with current move
	void setMarks(MarkSet const& marks);
	/// Adds a move at the current position
	void addMove(mstl::string const& san);
	/// Adds a move at the current position
	void addMove(Move const& move);
	/// Adds moves to main line
	void addMoves(MoveNodeP node);
	/// Adds moves to main line
	void addMoves(MoveList const& moves);
	/// Insert move after the current position
	bool exchangeMove(mstl::string const& san, Force flag = OnlyIfRemainsConsistent);
	/// Insert move after the current position
	bool exchangeMove(Move move, Force flag = OnlyIfRemainsConsistent);
	/// Replace all moves after current position with given move
	void replaceVariation(Move const& move);
	/// Replace all moves after current position with given move
	void replaceVariation(mstl::string const& san);
	/// Replace variation with given variation number, useful for changing a previously added variation
	void changeVariation(MoveNodeP node, unsigned variationNumber);
	/// Adds a move at the current position as a variation, and returns the variation number
	unsigned addVariation(Move const& move, move::Position position = move::Post);
	/// Adds a move at the current position as a variation, and returns the variation number
	unsigned addVariation(mstl::string const& san, move::Position position = move::Post);
	/// Adds a new variation at the current position
	unsigned addVariation(MoveNodeP node, move::Position position = move::Post);
	/// Adds a new variation at the current position
	unsigned addVariation(MoveList const& moves, move::Position position = move::Post);
	/// Merge a move as a variation
	void mergeVariation(Move const& move);
	/// Merge a move as a variation
	void mergeVariation(mstl::string const& san);
	/// Merge a new variation
	void mergeVariation(MoveNodeP node);
	/// Merge a variation at the current position
	void mergeVariation(MoveList const& moves, move::Position position = move::Post);
	/// Adds a new variation and promotes this variation one line up
	void newMainline(mstl::string const& san);
	/// Adds a new variation and promotes this variation one line up
	void newMainline(Move const& move);
	/// Promotes the given variation one line up
	void promoteVariation(unsigned oldVariationNumber, unsigned newVariationNumber = 0);
	/// Makes the specified variation the first variation
	void firstVariation(unsigned variationNumber);
	/// Removes the given variation
	void removeVariation(unsigned variationNumber);
	/// Removes all variations and mainline moves after the current position,
	/// or before the current position if @p position == Ante
	void truncateVariation(move::Position position = move::Post);
	/// Exchange the moves of given variation with main line moves,
	bool exchangeMoves(	unsigned variationNumber,
								unsigned movesToExchange,
								Force flag = OnlyIfRemainsConsistent);
	/// Insert the moves of given variation with main line moves,
	bool insertMoves(unsigned variationNumber, Force flag = OnlyIfRemainsConsistent);
	/// Removes all variations and mainline moves before next move,
	/// or before the current position if @p position == Ante
	bool stripMoves(move::Position position = move::Post);
	/// Merge given game (at current position) into current game.
	bool merge(	unsigned modificationPosition,
					Game const& game,
					position::ID startPosition,
					move::Order order,
					unsigned variationDepth,
					unsigned maximalVariationLength);
	/// Merge given games into current game.
	bool merge(	Game const& game1,
					Game const& game2,
					position::ID startPosition,
					move::Order order,
					unsigned variationDepth,
					unsigned maximalVariationLength);
	/// Remove all annotations.
	bool stripAnnotations();
	/// Remove all comments.
	bool stripComments();
	/// Remove all comments of given language code.
	bool stripComments(mstl::string const& lang);
	/// Copy comments from one language code to another language code.
	bool copyComments(mstl::string const& fromLang,
							mstl::string const& toLang,
							bool stripOriginal = false);
	/// Remove all move information.
	bool stripMoveInfo();
	/// Remove all marks.
	bool stripMarks();
	/// Remove all variations.
	bool stripVariations();
	/// Undo previous change.
	void undo();
	/// Redo previous undo.
	void redo();
	/// Remove all moves; and possibly set new start position.
	void clear(Board const* startPosition = 0);
	/// Clear merge results.
	void clearMergeResults();
	/// Setup this game.
	void setup(Board const& startPosition);
	/// Transpose game.
	bool transpose(Force flag = OnlyIfRemainsConsistent);
	/// Clean up variations.
	unsigned cleanupVariations();
	/// Set given variation folded/unfolded.
	void setFolded(edit::Key const& key, bool flag = true);
	/// Toggle fold flag of given variation.
	void toggleFolded(edit::Key const& key);
	/// Fold/unfold all variations.
	void setFolded(bool flag = true);
	/// Unfold variation if current ply is after first ply of an folded variation.
	void unfold();

	// modification methods

	/// Set the game start position from FEN.
	void setStartPosition(mstl::string const& fen);
	/// Set the game start position from IDN.
	void setStartPosition(unsigned idn);
	/// Set the subscriber for this game (normally a PGN display)
	void setSubscriber(SubscriberP subscriber);
	/// Release subscriber for this game.
	SubscriberP releaseSubscriber();
	/// Traverse whole game.
	void updateSubscriber(unsigned action = UpdateBoard | UpdatePgn);
	/// Traverse whole game.
	void refreshSubscriber(unsigned actions);
	/// Set undo level.
	void setUndoLevel(unsigned level, unsigned combinePredecessingMoves);
	/// Set game tags.
	void setTags(TagSet const& tags);
	/// Set game flags (should coincide with game flags in GameInfo).
	void setGameFlags(unsigned flags);
	/// Remove some flags from game flags.
	void removeFlags(unsigned flags);
	/// Reset game for next load.
	void resetForNextLoad(variant::Type variant);
	/// Set current language set.
	void setLanguages(LanguageSet const& set);
	/// Select all languages.
	void setAllLanguages();
	/// Set whether game is modified anymore.
	void setIsModified(bool flag);
	/// Set whether game is irreversible modified.
	void setIsIrreversible(bool flag);
	/// Clear undo stack.
	void clearUndo();
	/// Swap game specific data; useful after a game swap
	void swapGameSpecificData(Game& game);

	// undo - redo

	/// Get current undo action.
	Command undoCommand() const;
	/// Get current redo action.
	Command redoCommand() const;
	/// Get current rollback command.
	Command rollbackCommand() const;
	/// Start undo/rollback point.
	void startUndoPoint(Command command, Action action);
	/// End undo/rollback point.
	void endUndoPoint(unsigned action = UpdatePgn | UpdateBoard);

	void setup(	unsigned linebreakThreshold,
					unsigned linebreakMaxLineLengthMain,
					unsigned linebreakMaxLineLengthVar,
					unsigned linebreakMinCommentLength,
					unsigned displayStyle,
					unsigned moveInfoTypes,
					move::Notation moveStyle);

	Board const& getFinalBoard() const override;
	Board const& getStartBoard() const override;

	static void printMove(	Board const& board,
									Move const& move,
									mstl::string& result,
									move::Notation form,
									unsigned flags = ExportFormat);

	unsigned dumpMoves(mstl::string& result, unsigned flags);
	unsigned dumpMoves(mstl::string& result, unsigned length, unsigned flags);
	unsigned dumpHistory(mstl::string& result, protocol::ID protocol) const;
	void getHistory(History& result) const;

	bool finishLoad(variant::Type variant, mstl::string const* fen = 0);

	friend class Database;

private:

	enum State { Begin_Group, End_Group, Begin_Var, End_Var, Do_Move };

	enum UndoAction
	{
		Set_Annotation,
		Set_Trailing_Comment,
		Replace_Node,
		Replace_Prev_Node,
		Exchange_Next_Node,
		Truncate_Variation,
		Swap_Variations,
		Promote_Variation,
		Insert_Variation,
		Remove_Variation,
		Remove_Mainline,
		New_Mainline,
		Strip_Moves,
		Unstrip_Moves,
		Revert_Game,
		Set_Start_Position,
	};

	struct Undo;

	typedef mstl::vector<Undo*> UndoList;
	typedef termination::State FinalState;

	void doMove();
	void undoMove();
	void goToCurrentMove(bool forward) const;
	void tryMoveTo(edit::Key const& key);

	void getMoves(StringList& result, unsigned flags, move::Notation form);
	void getKeys(StringList& result);

	Undo& newUndo(UndoAction action, Command command);
	Undo* prevUndo(unsigned back = 1);
	void applyUndo(Undo& undo, bool redo);

	void insertUndo(UndoAction action, Command command);
	void insertUndo(UndoAction action, Command command, MoveNode* node);
	void insertUndo(UndoAction action, Command command, MoveNode* node, unsigned varNo);
	void insertUndo(UndoAction action, Command command, unsigned varNo);
	void insertUndo(UndoAction action, Command command, unsigned varNo1, unsigned varNo2);
	void insertUndo(UndoAction action, Command command, MoveNode* node, Board const& board);
	void insertUndo(	UndoAction action,
							Command command,
							Comment const& oldComment,
							Comment const& newComment,
							move::Position position);
	void insertUndo(	UndoAction action,
							Command command,
							Comment const& oldComment,
							Comment const& newComment);
	void insertUndo(	UndoAction action,
							Command command,
							MarkSet const& oldMarks,
							MarkSet const& newMarks);
	void insertUndo(	UndoAction action,
							Command command,
							Annotation const& oldAnnotation,
							Annotation const& newAnnotation);

	void replaceNode(MoveNode* newNode, Command command);
	void replacePrevNode(MoveNode* newNode, Command command);
	void exchangeNextNode(MoveNode* node, Command command);
	void insertVariation(MoveNode* variation, unsigned number);
	void unstripMoves(MoveNode* startNode, Board const& startBoard, edit::Key const& key);
	void revertGame(MoveNode* startNode, Command command);
	void resetGame(MoveNode* startNode, Board const& startBoard, edit::Key const& key);
	void moveVariation(unsigned from, unsigned to, Command command);
	void mergeVariation(Variation const& moves);
	bool mergeVariation(	Variation const& moves,
								move::Position position,
								Command command,
								unsigned updateFlags);
	bool mergeVariation(MoveNodeP node, move::Position position, Command command, unsigned updateFlags);
	void newMainline(MoveNode* node);
	void promoteVariation(unsigned oldVariationNumber, unsigned newVariationNumber, bool update);
	void removeMainline();
	bool updateLine();
	void updateFinalBoard();
	bool updateLanguageSet();
	void incrementChangeCounter();
	bool checkConsistency(MoveNode* node, Board& board, Force flag) const;
	MoveNode* findPosition(	Board wanted,
									MoveNode* node,
									Board board,
									unsigned depth,
									bool ignoreEnPassant) const;
	/// Return current move node.
	MoveNode* currentNode() const;

	Move parseMove(mstl::string const& san) const;

	edit::Root* buildEditNodes() const;

	mutable SubscriberP m_subscriber;

	struct EditorOptions
	{
		EditorOptions();

		unsigned				m_linebreakThreshold;
		unsigned				m_linebreakMaxLineLengthMain;
		unsigned				m_linebreakMaxLineLengthVar;
		unsigned				m_linebreakMinCommentLength;
		unsigned				m_displayStyle;
		unsigned				m_moveInfoTypes;
		move::Notation		m_moveStyle;
	};

	unsigned				m_id;
	MoveNode*			m_currentNode;
	edit::Root*			m_editNode;
	Board					m_currentBoard;
	mutable Board		m_finalBoard;
	edit::Key			m_currentKey;
	mutable edit::Key	m_previousKey;
	Eco					m_eco;
	UndoList				m_undoList;
	LanguageSet			m_languageSet;
	LanguageSet			m_wantedLanguages;
	unsigned				m_undoIndex;
	unsigned				m_maxUndoLevel;
	unsigned				m_combinePredecessingMoves;
	Command				m_undoCommand;
	Command				m_redoCommand;
	Command				m_rollbackCommand;
	uint32_t				m_flags;
	bool					m_isIrreversible;
	bool					m_isModified;
	bool					m_wasModified;
	mutable bool		m_threefoldRepetionDetected;
	mutable bool		m_fivefoldRepetionDetected;
	FinalState			m_termination;
	uint16_t				m_lineBuf[opening::Max_Line_Length][2];
	mutable Line		m_line;
	mutable bool		m_changed;
	EditorOptions		m_editorOptions;
	MergeResults		m_mergeResults;

#ifdef DB_DEBUG_GAME
	MoveNode* m_backupNode;
	edit::Key m_backupKey;
	void beginBackup();
	void endBackup();
#endif

	static unsigned m_gameId;
	static mstl::string m_delim;
};

} // namebase db

#include "db_game.ipp"

#endif // _db_game_included

// vi:set ts=3 sw=3:
