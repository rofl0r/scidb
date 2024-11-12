// ======================================================================
// Author : $Author$
// Version: $Revision: 36 $
// Date   : $Date: 2011-06-13 20:30:54 +0000 (Mon, 13 Jun 2011) $
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
// Copyright: (C) 2011 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _db_edit_node_included
#define _db_edit_node_included

#include "db_board.h"
#include "db_move.h"
#include "db_eco.h"
#include "db_edit_key.h"
#include "db_comment.h"
#include "db_annotation.h"
#include "db_mark_set.h"

#include "m_vector.h"
#include "m_map.h"
#include "m_string.h"

namespace db {

class Comment;
class Annotation;
class MarkSet;
class MoveNode;
class TagSet;

namespace edit {

class Visitor;
class Languages;
class Root;
class Variation;


class Node
{
public:

	enum Type
	{
		TRoot, TOpening, TLanguages,						// root level (unused)
		TAction,													// root level (used)
		TMove, TDiagram, TVariation,						// variation level
		TPly, TAnnotation, TMarks, TComment, TSpace,	// move level
	};

	enum Bracket { Blank, Open, Close, Fold, Empty, Start };

	typedef mstl::vector<Node const*> List;
	typedef mstl::map<mstl::string,unsigned> LanguageSet;

	virtual ~Node() throw() = 0;

	virtual bool operator==(Node const* node) const;
	bool operator!=(Node const* node) const;

	virtual Type type() const = 0;

	virtual void visit(Visitor& visitor) const = 0;

	static void visit(Visitor& visitor, List const& nodes, TagSet const& tags);

protected:

	struct Spacing;
	struct Work;

	static char const PrefixDiagram	= 'd';
	static char const PrefixComment	= 'c';

	bool isRoot() const;
};


class KeyNode : public Node
{
public:

	typedef mstl::vector<KeyNode const*> List;

	KeyNode(Key const& key);
	KeyNode(Key const& key, char prefix);

	bool operator==(KeyNode const* node) const;
	bool operator!=(KeyNode const* node) const;

	bool operator<(KeyNode const* node) const;
	bool operator>(KeyNode const* node) const;

	Key const& key() const;
	virtual Key const& startKey() const;
	virtual Key const& endKey() const;

protected:

	Key m_key;
};


class Action : public Node
{
public:

	enum Command { Clear, Insert, Replace, Remove, Finish };

	Action(Command command);
	Action(Command command, unsigned level);
	Action(Command command, unsigned level, Key const& beforeKey);
	Action(Command command, unsigned level, Key const& startKey, Key const& endKey);

	Type type() const;

	void visit(Visitor& visitor) const;

private:

	Command	m_command;
	Key		m_key1;
	Key		m_key2;
	unsigned	m_level;
};


class Root : public Node
{
public:

	Root();
	~Root() throw();

	Type type() const;

	void visit(Visitor& visitor) const;
	void difference(Root const* root, List& nodes) const;

	static Root* makeList(	TagSet const& tags,
									uint16_t idn,
									Eco eco,
									db::Board const& startBoard,
									MoveNode const* node,
									unsigned linebreakThreshold,
									unsigned linebreakMaxLineLength);
	static Root* makeList(	TagSet const& tags,
									uint16_t idn,
									Eco eco,
									db::Board const& startBoard,
									LanguageSet const& langSet,
									LanguageSet const& wantedLanguages,
									MoveNode const* node,
									unsigned linebreakThreshold,
									unsigned linebreakMaxLineLength,
									unsigned linebreakMaxLineLengthVar,
									unsigned linebreakMinCommentLength,
									unsigned displayStyle);

	Node* newAction(Action::Command command) const;
	Node* newAction(Action::Command command, unsigned level) const;
	Node* newAction(Action::Command command, unsigned level, Key const& beforeKey) const;
	Node* newAction(Action::Command command, unsigned level, Key const& startKey, Key const& endKey)const;

private:

	static void makeList(Work& work, KeyNode::List& result, MoveNode const* node);

	Node*				m_opening;
	Node* 			m_languages;
	Variation*		m_variation;
	result::ID		m_result;
	mutable List	m_nodes;
};


class Opening : public Node
{
public:

	Opening(Board const& startBoard, uint16_t idn, Eco eco);

	bool operator==(Node const* node) const;

	Type type() const;

	void visit(Visitor& visitor) const;

private:

	Board		m_board;
	uint16_t	m_idn;
	db::Eco	m_eco;
};


class Languages : public Node
{
public:

	Languages(MoveNode const* root = 0);

	bool operator==(Node const* node) const;

	Type type() const;
	LanguageSet const& langSet() const;

	void visit(Visitor& visitor) const;

private:

	LanguageSet m_langSet;
};


class Variation : public KeyNode
{
public:

	Variation(Key const& key);
	Variation(Key const& key, Key const& succ);
	~Variation() throw();

	bool operator==(Node const* node) const;

	bool empty() const;

	Type type() const;

	Key const& startKey() const;
	Key const& endKey() const;
	Key const& successor() const;

	void visit(Visitor& visitor) const;

private:

	friend class Root;

	void difference(Root const* root, Variation const* var, unsigned level, Node::List& nodes) const;

	List	m_list;
	Key	m_succ;
};


class Ply : public Node
{
public:

	Ply(db::MoveNode const* move, unsigned moveno = 0);

	bool operator==(Node const* node) const;

	Type type() const;
	unsigned moveNo() const;
	db::Move const& move() const;

	void visit(Visitor& visitor) const;

private:

	unsigned m_moveNo;
	db::Move m_move;
};


class Move : public KeyNode
{
public:

	typedef Node::List List;

	Move(Work& work, MoveNode const* move);
	Move(Work& work, db::Comment const& comment);
	Move(Work& work);

	Move(Key const& key);
	Move(Spacing& spacing, Key const& key, unsigned moveNumber, MoveNode const* move);

	~Move() throw();

	bool operator==(Node const* node) const;

	Type type() const;

	Ply const* ply() const;

	void visit(Visitor& visitor) const;

private:

	friend class Root;

	List	m_list;
	Ply*	m_ply;
};


class Diagram : public KeyNode
{
public:

	Diagram(Work& work, color::ID fromColor);

	bool operator==(Node const* node) const;

	Type type() const;

	void visit(Visitor& visitor) const;

private:

	db::Board	m_board;
	color::ID	m_fromColor;
};


class Comment : public Node
{
public:

	Comment(db::Comment const& comment, move::Position position, bool atStart = false);

	bool operator==(Node const* node) const;

	Type type() const;

	void visit(Visitor& visitor) const;

private:

	move::Position	m_position;
	bool				m_atStart;
	db::Comment		m_comment;
};


class Annotation : public Node
{
public:

	Annotation(db::Annotation const& annotation, bool deleteDiagram = false);

	bool operator==(Node const* node) const;

	Type type() const;

	void visit(Visitor& visitor) const;

private:

	db::Annotation m_annotation;
};


class Marks : public Node
{
public:

	Marks(MarkSet const& marks);

	bool operator==(Node const* node) const;

	Type type() const;

	void visit(Visitor& visitor) const;

private:

	db::MarkSet m_marks;
};


class Space : public Node
{
public:

	Space(Bracket bracket = Blank);
	Space(unsigned level);

	bool operator==(Node const* node) const;

	Type type() const;

	void visit(Visitor& visitor) const;

private:

	int		m_level;
	Bracket	m_bracket;
};


class Visitor
{
public:

	typedef Node::LanguageSet LanguageSet;
	typedef Node::Bracket Bracket;

	virtual ~Visitor() throw();

	virtual void clear() = 0;
	virtual void insert(unsigned level, Key const& beforeKey) = 0;
	virtual void replace(unsigned level, Key const& startKey, Key const& endKey) = 0;
	virtual void remove(unsigned level, Key const& startKey, Key const& endKey) = 0;
	virtual void finish(unsigned level) = 0;

	virtual void opening(Board const& startBoard, uint16_t idn, Eco const& eco) = 0;
	virtual void languages(LanguageSet const& languages) = 0;
	virtual void move(unsigned moveNo, db::Move const& move) = 0;
	virtual void position(db::Board const& board, color::ID fromColor) = 0;
	virtual void comment(move::Position position, bool atStartOfVariation, db::Comment const& comment) = 0;
	virtual void annotation(db::Annotation const& annotation) = 0;
	virtual void marks(MarkSet const& marks) = 0;
	virtual void space(Bracket bracket) = 0;
	virtual void linebreak(unsigned level) = 0;

	virtual void start(result::ID result) = 0;
	virtual void finish(result::ID result) = 0;

	virtual void startVariation(Key const& key, Key const& startKey, Key const& endKey) = 0;
	virtual void endVariation(Key const& key, Key const& startKey, Key const& endKey) = 0;

	virtual void startMove(Key const& key) = 0;
	virtual void endMove(Key const& key) = 0;

	virtual void startDiagram(Key const& key) = 0;
	virtual void endDiagram(Key const& key) = 0;
};

} // namespace edit
} // namespace db

#include "db_edit_node.ipp"

#endif // _db_edit_node_included

// vi:set ts=3 sw=3:
