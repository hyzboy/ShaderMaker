#pragma once

namespace shader_lib
{
	// 注：我们只用于简单的解晰，并不需要对整个GLSL进行词法分析

	enum class GLSLToken
	{
		None=0,
        End,
		
		WhiteSpace,			// ' ','\t'
		NewLine,			// '\r\n'
        OnelineComment,		// // \n
        MultilineComment,   // /* */

        // Atoms
		Identifier,			// abc123,
		IntConstant,		// 1234
		FloatConstant,		// 12.34
		StringConstant,		// "123"
        BitsConstant,        // 0xFFFF
		
        // Math operators
		Plus,				// +
		Minus,				// -
		Star,				// *
		Slash,				// /
		Percent,			// %
		Handle,				// #
        Amp,                // &
		Not,				// !

		AddAssign,			// +=
        SubAssign,          // -=
        MulAssign,          // *=
        DivAssign,          // /=
        ModAssign,          // %=

        OrAssign,           // |=
        AndAssign,          // &=
        XorAssign,          // ^=
        ShiftLeftAssign,    // <<=
        ShiftRightAssign,   // >>=

        Inc,                // ++
        Dec,                // --

        Dot,                // .

        // Statement tokens
        Assignment,          // =
        EndStatement,        // ;
        ListSeparator,       // ,

        StartStatementBlock, // {
        EndStatementBlock,   // }
        OpenParanthesis,     // (
        CloseParanthesis,    // )
        OpenBracket,         // [
        CloseBracket,        // ]

        // Bitwise operators
        BitOr,               // |
        BitNot,              // ~
        BitXor,              // ^
        BitShiftLeft,        // <<
        BitShiftRight,       // >>

        // Compare operators
        Equal,               // ==
        NotEqual,            // !=
        LessThan,            // <
        GreaterThan,         // >
        LessThanOrEqual,     // <=
        GreaterThanOrEqual,  // >=

        Question,           // ?
        Colon,              // :
		Or,					// ||
		And,				// &&

        // Reserved keywords
        If,                 // if
        Else,               // else
        For,                // for
		Do,					// do
        While,              // while
        Goto,               // goto
		Break,				// break
		Continue,			// continue
		Switch,
		Case,
		Default,			// default

        Import,             // import
		Include,			// include

		True,
		False,

		In,
		Out,
		InOut,

		Struct,
		Uniform,

		Const,
		Shared,

		Coherent,
		Volatile,
		Restrict,
		ReadOnly,
		WriteOnly,

		Precision,

		Invariant,

		Patch,

		Flat,
		Noperspective,
		Smooth,

		Subroutine,
			
		Void,
		Boolean,
		Integer,
		UInteger,
		Float,
		Double,

		vec,		vec1,		vec2,		vec3,		vec4,
		bvec,		bvec1,		bvec2,		bvec3,		bvec4,
		ivec,		ivec1,		ivec2,		ivec3,		ivec4,
		uvec,		uvec1,		uvec2,		uvec3,		uvec4,
		dvec,		dvec1,		dvec2,		dvec3,		dvec4,

		mat2,	mat3,	mat4,
		mat2x2,	mat2x3,	mat2x4,
		mat3x2,	mat3x3,	mat3x4,
		mat4x2,	mat4x3,	mat4x4,

		row_major,
		column_major,

		origin_upper_left,
		pixel_center_integer,

		sampler,
		sampler1D,
		sampler2D,
		sampler3D,
		samplerCube,
		sampler2DRect,
		sampler1DArray,
		sampler2DArray,
		samplerCubeArray,
		samplerBuffer,
		sampler2DMS,
		sampler2DMSArray,

		sampler1DShadow,
		sampler2DShadow,
		samplerCubeShadow,
		sampler2DRectShadow,
		sampler1DArrayShadow,
		sampler2DArrayShadow,
		samplerCubeArrayShadow,

		buffer,

		depth_any,
		depth_greater,
		depth_less,
		depth_unchanged,
	};//enum class GLSLToken
}//namespace shader_lib
