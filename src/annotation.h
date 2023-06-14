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
			Number, 
			boost::recursive_wrapper<IdentifierNode<Number>>, 
			boost::recursive_wrapper<UnaryOperationNode<Number>>, 
			boost::recursive_wrapper<OperationNode<Number>>, 
			boost::recursive_wrapper<MixedDivivsionNode<Number>>, 
			boost::recursive_wrapper<ImplicitStringMulNode<Number>>, 
			boost::recursive_wrapper<ImplicitDivMulNode<Number>>, 
			boost::recursive_wrapper<FunctionCallNode<Number>>, 
			boost::recursive_wrapper<NoFencesFunctionCallNode<Number>>, 
			boost::recursive_wrapper<ExpressionNode<Number>>>
			Operand;

		Annotation(std::u32string::iterator _first, std::u32string::iterator _last, ElementId& _id) : 
			first(_first),
			last(_last),
			id(_id)
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

		void operator()(ExpressionNode<Number>& expr, std::u32string::iterator pos) const
		{
			UpdatePosition(pos, expr);
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

		void operator()(FunctionCallNode<Number>& op, std::u32string::iterator pos) const
		{
			UpdatePosition(pos, op);
		}

		void operator()(NoFencesFunctionCallNode<Number>& op, std::u32string::iterator pos) const
		{
			UpdatePosition(pos, op);
		}

		template<typename Num>
		struct OperandVisitor
		{
			OperandVisitor(Annotation<Num> const* _annotation, std::u32string::iterator _iter) : annotation(_annotation), iter(_iter)
			{
			}
			
			//The visitor's functor for OperationNode.
			void operator()(OperationNode<Num> const& op) const
			{
				annotation->UpdatePosition(iter, op);
			}

			void operator()(UnaryOperationNode<Num> const& op) const
			{
			}

			void operator()(MixedDivivsionNode<Num> const& op) const
			{
			}

			void operator()(ImplicitStringMulNode<Number>& op) const
			{
			}

			void operator()(ImplicitDivMulNode<Number>& op) const
			{
			}

			void operator()(IdentifierNode<Num> const& op) const
			{
				annotation->UpdatePosition(iter, op);
			}

			void operator()(FunctionCallNode<Num> const& op) const
			{
				annotation->UpdatePosition(iter, op);
			}

			void operator()(NoFencesFunctionCallNode<Num> const& op) const
			{
				annotation->UpdatePosition(iter, op);
			}

			void operator()(ExpressionNode<Num> const& op) const
			{
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

		ElementId id;
		std::u32string::iterator first;
		std::u32string::iterator last;
	};
}

#endif
