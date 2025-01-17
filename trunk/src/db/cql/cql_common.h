// ======================================================================
// Author : $Author$
// Version: $Revision: 769 $
// Date   : $Date: 2013-05-10 22:26:18 +0000 (Fri, 10 May 2013) $
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
// Copyright: (C) 2013 Gregor Cramer
// ======================================================================

// ======================================================================
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// ======================================================================

#ifndef _cql_common_included
#define _cql_common_included

#include "db_common.h"

namespace mstl { class string; }

namespace cql {

namespace flags
{
	enum
	{
		IsFinalPosition	= 1 << 0,
		IsInsideVariation	= 1 << 1,
	};
};

namespace error
{
	enum Type
	{
		No_Error,
		Unexpected_Comma,
		Missing_Parameter,
		Invalid_Keyword,
		Invalid_Relation_Keyword,
		Range_Expected,
		Pattern_Expected,
		Integer_Expected,
		Invalid_Square_Designator,
		Invalid_Evaluation_Mode,
		Positive_Integer_Expected,
		Double_Quote_Expected,
		Unterminated_String,
		Empty_String_Not_Allowed,
		Invalid_Date,
		Syntax_Error_In_Date,
		Empty_Date_Range,
		Illegal_Date,
		Illegal_Date_Offset,
		Invalid_Eco_Code,
		Invalid_Rating_Type,
		Invalid_Country_Code,
		Illegal_Country_Code,
		Invalid_Event_Mode,
		Invalid_Event_Type,
		Invalid_Game_Flag,
		Invalid_Tag_Name,
		Invalid_Gender,
		Invalid_Result,
		Invalid_Termination,
		Invalid_Time_Mode,
		Invalid_Title,
		Invalid_Variant,
		Invalid_Fen,
		Invalid_Promotion_Ranks,
		Invalid_FICS_Position,
		Invalid_IDN,
		Invalid_Range_Argument,
		Invalid_Evaluation_View,
		Invalid_Special_Marker,
		Position_Number_Expected,
		Integer_Out_Of_Range,
		Unexpected_Token,
		Left_Parenthesis_Expected,
		Right_Parenthesis_Expected,
		Trailing_Characters,
		Keyword_Match_Expected,
		Keyword_Position_Expected,
		Position_List_Expected,
		Keyword_Match_Or_Position_Expected,
		Relation_List_Expected,
		Position_List_Is_Not_Allowed,
		Unmatched_Bracket,
		Empty_Piece_Designator,
		Invalid_Designator,
		Any_Fyle_Not_Allowed_In_Range,
		Any_Rank_Not_Allowed_In_Range,
		Invalid_Fyle_In_Square_Designator,
		Invalid_Rank_In_Square_Designator,
		Missing_Rank_In_Square_Designator
	};

} // namespace error

namespace piece {

	enum ID
	{
		WK = 1 << 0,
		WQ = 1 << 1,
		WR = 1 << 2,
		WB = 1 << 3,
		WN = 1 << 4,
		WP = 1 << 5,

		BK = 1 << 6,
		BQ = 1 << 7,
		BR = 1 << 8,
		BB = 1 << 9,
		BN = 1 << 10,
		BP = 1 << 11,

		E = 1 << 12,
	};
}

namespace country
{
	db::country::Code lookupIso3166_2(mstl::string const& s); // lookup ISO 3166-2 country code

} // namespace country

namespace transformation
{
	enum
	{
		Flip_Color				= 1 << 0,

		Flip_Main_Diagonal	= 1 << 1,
		Flip_Off_Diagonal		= 1 << 2,
		Flip_Diagonal			= Flip_Main_Diagonal | Flip_Off_Diagonal,
		Flip_Vertical			= 1 << 3,
		Flip_Horizontal		= 1 << 4,
		Flip_Both				= Flip_Vertical | Flip_Horizontal,
		Flip						= Flip_Diagonal | Flip_Both,

		Shift_Horizontal		= 1 << 5,
		Shift_Vertical			= 1 << 6,
		Shift_Both				= Shift_Horizontal | Shift_Vertical,
		Shift_Main_Diagonal	= 1 << 7,
		Shift_Off_Diagonal	= 1 << 8,
		Shift_Diagonal			= Shift_Main_Diagonal | Shift_Off_Diagonal,
		Shift						= Shift_Both | Shift_Diagonal,
	};
}

} // namespace cql

#endif // _cql_common_included

// vi:set ts=3 sw=3:
