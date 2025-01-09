#ifndef ANNOTATION_H
#define ANNOTATION_H

#include "ast.h"

namespace yutovo_calculator
{
	//Annotation for nodes. Used in purposes of searching a node by a text position.
	template<typename Number>
	struct Annotation
	{
		typedef boost::variant<
			boost::recursive_wrapper<NumberNode<Number>>, 
			boost::recursive_wrapper<IdentifierNode<Number>>, 
			boost::recursive_wrapper<UnaryOperationNode<Number>>, 
			boost::recursive_wrapper<OperationNode<Number>>, 
			boost::recursive_wrapper<PostfixOperationNode<Number>>, 
			boost::recursive_wrapper<MixedDivivsionNode<Number>>, 
			boost::recursive_wrapper<ImplicitStringMulNode<Number>>, 
			boost::recursive_wrapper<ImplicitDivMulNode<Number>>, 
			boost::recursive_wrapper<ImplicitMulNode<Number>>, 
			boost::recursive_wrapper<ImplicitFractionMulNode<Number>>, 
			boost::recursive_wrapper<ImplicitFunctionMulNode<Number>>, 
			boost::recursive_wrapper<ImplicitPostFunctionMulNode<Number>>, 
			boost::recursive_wrapper<FunctionCallNode<Number>>, 
			boost::recursive_wrapper<FunctionCallStringNode<Number>>, 
			boost::recursive_wrapper<NoFencesFunctionCallNode<Number>>, 
			boost::recursive_wrapper<CompareNode<Number>>, 
			boost::recursive_wrapper<LoopNode<Number>>, 
			boost::recursive_wrapper<ExpressionNode<Number>>>
			Operand;

		Annotation(std::u32string::iterator _first, std::u32string::iterator _last, LogicalId& _id, ParserContext** _parser_context) : 
			id(_id),
			first(_first),
			last(_last),
			parser_context(_parser_context)
		{
		}

		//Updates the position in the expression.
		void UpdatePosition(std::u32string::iterator p, ExpressionPosition const& position) const
		{
			UpdatePosition(p, (ExpressionPosition&)position);
		}
		
		//Updates the position in the expression.
		void UpdatePosition(std::u32string::iterator p, ExpressionPosition& position) const
		{
			CheckBreak(*parser_context);

			int i = 0, j = 0;
			std::u32string::iterator k = first;
			
			while (k != p)
			{
				char ch = *k;
				if (ch == '\r')
				{
					++k;
					if (ch == '\n')
						++k;
					++j;
				}
			
				++k;
				++i;
			}
			
			position.id = id;
			position.pos = i;
			position.line = j;
		}
		
		void operator()(Operand& op, std::u32string::iterator pos) const
		{
			boost::apply_visitor(OperandVisitor<Number>(this, pos), op);
		}

		void operator()(IdentifierNode<Number>& op, std::u32string::iterator pos) const
		{
			UpdatePosition(pos, op);
		}

		void operator()(NumberNode<Number>& op, std::u32string::iterator pos) const
		{
			UpdatePosition(pos, op);
		}

		void operator()(ExpressionNode<Number>& expr, std::u32string::iterator pos) const
		{
			UpdatePosition(pos, expr);
		}

		void operator()(UnaryOperationNode<Number>& op, std::u32string::iterator pos) const
		{
			UpdatePosition(pos, op);
		}

		void operator()(PostfixOperationNode<Number>& op, std::u32string::iterator pos) const
		{
			UpdatePosition(pos, op);
		}

		void operator()(OperationNode<Number>& op, std::u32string::iterator pos) const
		{
			UpdatePosition(pos, op);
		}

		void operator()(MixedDivivsionNode<Number>& op, std::u32string::iterator pos) const
		{
			UpdatePosition(pos, op);
		}

		void operator()(ImplicitStringMulNode<Number>& op, std::u32string::iterator pos) const
		{
			UpdatePosition(pos, op);
		}

		void operator()(ImplicitDivMulNode<Number>& op, std::u32string::iterator pos) const
		{
			UpdatePosition(pos, op);
		}

		void operator()(ImplicitMulNode<Number>& op, std::u32string::iterator pos) const
		{
			UpdatePosition(pos, op);
		}

		void operator()(ImplicitFractionMulNode<Number>& op, std::u32string::iterator pos) const
		{
			UpdatePosition(pos, op);
		}

		void operator()(ImplicitFunctionMulNode<Number>& op, std::u32string::iterator pos) const
		{
			UpdatePosition(pos, op);
		}

		void operator()(ImplicitPostFunctionMulNode<Number>& op, std::u32string::iterator pos) const
		{
			UpdatePosition(pos, op);
		}

		void operator()(FunctionCallNode<Number>& op, std::u32string::iterator pos) const
		{
			UpdatePosition(pos, op);
		}

		void operator()(FunctionCallStringNode<Number>& op, std::u32string::iterator pos) const
		{
			UpdatePosition(pos, op);
		}

		void operator()(NoFencesFunctionCallNode<Number>& op, std::u32string::iterator pos) const
		{
			UpdatePosition(pos, op);
		}

		void operator()(CompareNode<Number>& op, std::u32string::iterator pos) const
		{
			UpdatePosition(pos, op);
		}

		void operator()(LoopNode<Number>& op, std::u32string::iterator pos) const
		{
			UpdatePosition(pos, op);
		}

		void operator()(VariableNode<Number>& op, std::u32string::iterator pos) const
		{
			UpdatePosition(pos, op);
		}

		void operator()(DefinitionNode<Number>& op, std::u32string::iterator pos) const
		{
			UpdatePosition(pos, op);
		}

		template<typename Num>
		struct OperandVisitor
		{
			OperandVisitor(Annotation<Num> const* _annotation, std::u32string::iterator _iter) : annotation(_annotation), iter(_iter)
			{
			}
			
			void operator()(OperationNode<Num> const& op) const
			{
				annotation->UpdatePosition(iter, op);
			}

			void operator()(NumberNode<Num> const& op) const
			{
				annotation->UpdatePosition(iter, op);
			}

			void operator()(UnaryOperationNode<Num> const& op) const
			{
				annotation->UpdatePosition(iter, op);
			}

			void operator()(PostfixOperationNode<Num> const& op) const
			{
				annotation->UpdatePosition(iter, op);
			}

			void operator()(MixedDivivsionNode<Num> const& op) const
			{
				annotation->UpdatePosition(iter, op);
			}

			void operator()(ImplicitStringMulNode<Number>& op) const
			{
				annotation->UpdatePosition(iter, op);
			}

			void operator()(ImplicitDivMulNode<Number>& op) const
			{
				annotation->UpdatePosition(iter, op);
			}

			void operator()(ImplicitMulNode<Number>& op) const
			{
				annotation->UpdatePosition(iter, op);
			}

			void operator()(ImplicitFractionMulNode<Number>& op) const
			{
				annotation->UpdatePosition(iter, op);
			}

			void operator()(ImplicitFunctionMulNode<Number>& op) const
			{
				annotation->UpdatePosition(iter, op);
			}

			void operator()(ImplicitPostFunctionMulNode<Number>& op) const
			{
				annotation->UpdatePosition(iter, op);
			}

			void operator()(IdentifierNode<Num> const& op) const
			{
				annotation->UpdatePosition(iter, op);
			}

			void operator()(FunctionCallNode<Num> const& op) const
			{
				annotation->UpdatePosition(iter, op);
			}

			void operator()(FunctionCallStringNode<Num> const& op) const
			{
				annotation->UpdatePosition(iter, op);
			}

			void operator()(NoFencesFunctionCallNode<Num> const& op) const
			{
				annotation->UpdatePosition(iter, op);
			}

			void operator()(CompareNode<Num> const& op) const
			{
				annotation->UpdatePosition(iter, op);
			}

			void operator()(LoopNode<Num> const& op) const
			{
				annotation->UpdatePosition(iter, op);
			}

			void operator()(ExpressionNode<Num> const& op) const
			{
				annotation->UpdatePosition(iter, op);
			}

			void operator()(DefinitionNode<Num> const& op) const
			{
				annotation->UpdatePosition(iter, op);
			}

			void operator()(VariableNode<Num> const& op) const
			{
				annotation->UpdatePosition(iter, op);
			}

			void operator()(Num const& op) const
			{
				//((Number)op).SetPrecision(10);
			}
			
			typedef void result_type;
			Annotation<Num> const* annotation;
			std::u32string::iterator iter;
		};

		template<typename, typename>
		struct result
		{
			typedef void type;
		};

		LogicalId id;
		std::u32string::iterator first;
		std::u32string::iterator last;
		ParserContext** parser_context;
	};
}

#endif
