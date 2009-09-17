#include "functionimpl.h"
#include "functionfactory.h"
#include "value.h"
#include "vector.h"
#include "variables.h"

#include <KDebug>
#include <KLocale>
#include <container.h>

struct FunctionParametric : public FunctionImpl
{
	explicit FunctionParametric(const Expression &e, Variables* v) : FunctionImpl(e, v) {}
	FunctionParametric(const FunctionParametric &fx) : FunctionImpl(fx) {}
	
	void updatePoints(const QRect& viewport);
	QPair<QPointF, QString> calc(const QPointF& dp);
	QLineF derivative(const QPointF& p) const;
	virtual FunctionImpl* copy() { return new FunctionParametric(*this); }
	
	static QStringList supportedBVars() { return QStringList("t"); }
	QStringList boundings() const { return supportedBVars(); }
};
REGISTER_FUNCTION(FunctionParametric)

void FunctionParametric::updatePoints(const QRect& viewport)
{
	Q_UNUSED(viewport);
	Q_ASSERT(func.expression().isCorrect());
	if(int(resolution())==points.capacity())
		return;
	
	double ulimit=uplimit(2*M_PI);
	double dlimit=downlimit(0);
	
	if(ulimit<=dlimit) {
		kDebug() << "Cannot have downlimit ≥ uplimit";
		return;
	}
	
	points.clear();
	points.reserve(resolution());
	
	func.variables()->modify("t", 0.);
	Cn *theT = (Cn*) func.variables()->value("t");
	
	double inv_res= double((ulimit-dlimit)/resolution());
	double final=ulimit-inv_res;
	for(double t=dlimit; t<final; t+=inv_res) {
		theT->setValue(t);
		Expression res=func.calculate();
		
		Object* vo=res.tree();
// 		objectWalker(vo);
		if(vo->type()==Object::vector) {
			Vector* v=static_cast<Vector*>(vo);
			bool valid=v->size()==2
				&& v->at(0)->type()==Object::value
				&& v->at(1)->type()==Object::value;
			
			if(valid) {
				Cn* x=static_cast<Cn*>(v->at(0));
				Cn* y=static_cast<Cn*>(v->at(1));
				addValue(QPointF(x->value(), y->value()));
			}
		}
	}
}

QPair<QPointF, QString> FunctionParametric::calc(const QPointF& p)
{
	func.variables()->modify("t", 0.);
	Expression res=func.calculate();
	Object* vo=res.tree();
	if(vo->type()!=Object::vector) {
		m_err += i18nc("if the specified function is not a vector",
					  "The parametric function does not return a vector");
	} else {
		Vector* v=static_cast<Vector*>(vo);
		if(v->size()!=2)
			m_err += i18nc("If it is a vector but the wrong size. We work in R2 here",
						   "We want a 2 dimension vector");
		else if(v->at(0)->type()!=Object::value || v->at(1)->type()!=Object::value)
			m_err += i18nc("The vector has to be composed by integer members",
						   "The parametric function items should be scalars");
	}
	return QPair<QPointF, QString>(p, QString());
}

QLineF FunctionParametric::derivative(const QPointF& p) const
{
	return QLineF();
}