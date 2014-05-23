/*!
 * @author Tuczi
 * @version 0.0.1
 * You must install gmp ( http://gmplib.org/ ) and mpfr ( http://www.mpfr.org/ ).
 */
 
#ifndef INTERVAL_HPP_INCLUDED
#define INTERVAL_HPP_INCLUDED

#include <string>
#include <algorithm>
#include <math.h>
#include <sstream>
#include <ostream>
#include <stdlib.h> 

#include <fenv.h> 
#include <gmp.h>
#include <mpfr.h>
#include "../../lib/mpreal.h"

/*!
 * @brief to controle float round mode
 */
#define ROUND_UP mpfr::mpreal::set_default_rnd(MPFR_RNDD)
#define ROUND_DOWN mpfr::mpreal::set_default_rnd(MPFR_RNDU)
#define ROUND_NEAREST mpfr::mpreal::set_default_rnd(MPFR_RNDN)

//@todo remove
//#define PRECISION 10
/*!
 * @brief Class and arithmetic for proper intervals
 */
namespace tuczi 
{
	typedef mpfr::mpreal point;
	/*!
	 * @brief class to represent an interval
	 */ 
	template <int PRECISION=16> class proper_interval
	{
		protected:
			/*! 
			 * @brief ends' of interval
			 */
			point *a, *b;
		public:
			/*!
			 * Simple constructors
			 */
			proper_interval(point p1, point p2);
			proper_interval(const char*, const char*);
			proper_interval(const proper_interval<PRECISION>&);
			proper_interval<PRECISION>& operator =(const proper_interval<PRECISION>&); 
						
			proper_interval(point p=0): proper_interval<PRECISION>(p, p) { }
			proper_interval(const char* str): proper_interval<PRECISION>(str, str) { }
			proper_interval(const std::string& s): proper_interval<PRECISION>(s.c_str()) { }
			proper_interval(const std::string& a, const std::string& b): proper_interval<PRECISION>(a.c_str(), b.c_str()) { }
			
			virtual ~proper_interval() { delete a; delete b;}//! @todo work with correct = operator
			
			point width();
			
			const proper_interval<PRECISION> operator +(const proper_interval<PRECISION>&);
			const proper_interval<PRECISION> operator -(const proper_interval<PRECISION>&);
			const proper_interval<PRECISION> operator *(const proper_interval<PRECISION>&);
			const proper_interval<PRECISION> operator /(const proper_interval<PRECISION>&);
			
			proper_interval<PRECISION>& operator +=(const proper_interval<PRECISION>&); 
			proper_interval<PRECISION>& operator -=(const proper_interval<PRECISION>&);
			proper_interval<PRECISION>& operator *=(const proper_interval<PRECISION>&);
			proper_interval<PRECISION>& operator /=(const proper_interval<PRECISION>&);
			
			template <int _PRECISION>
			friend std::ostream& operator<<(std::ostream&, proper_interval<_PRECISION>&);
			
			/*!
			 * @todo
			friend proper_interval square(const proper_interval&);
			friend proper_interval sqrt(const proper_interval&);
			
			friend proper_interval monotonicallyIncreasingFunction(proper_interval&, point(*)(point)); //! @brief interval extension of monotically increacing function on interval [a,b]
			friend proper_interval monotonicallyDecreasingFunction(proper_interval&, point(*)(point)); //! @brief interval extension of monotically decreacing function on interval [a,b]
			*/
	};
	
	template <int PRECISION> class dividing_exception
	{
		public: 
			proper_interval<PRECISION> interval;
			
			dividing_exception(const proper_interval<PRECISION>& _interval): interval(_interval) { }
	};
	
	template<int PRECISION> 
	proper_interval<PRECISION>::proper_interval(point p1, point p2)
	{
		mpfr::mpreal::set_default_prec(mpfr::digits2bits(PRECISION));
		ROUND_DOWN;
		a = new point(p1);
		ROUND_UP;
		b = new point(p2);
		
		ROUND_NEAREST;
	}

	template<int PRECISION> 
	proper_interval<PRECISION>::proper_interval(const char *str1,const char *str2)
	{
		mpfr::mpreal::set_default_prec(mpfr::digits2bits(PRECISION));
		ROUND_DOWN;
		a = new point(str1);
		ROUND_UP;
		b = new point(str2);
		
		ROUND_NEAREST;
	}
	
	template<int PRECISION> 
	proper_interval<PRECISION>::proper_interval(const proper_interval<PRECISION>& interval)
	{
		mpfr::mpreal::set_default_prec(mpfr::digits2bits(PRECISION));
		ROUND_DOWN;
		a = new point(*interval.a);
		ROUND_UP;
		b = new point(*interval.b);
		
		ROUND_NEAREST;
	}
	
	template<int PRECISION> 
	proper_interval<PRECISION>& proper_interval<PRECISION>::operator =(const proper_interval<PRECISION>& interval)
	{
		if(this == &interval) 
			return *this;
		
		delete a; delete b;
			
		a = new point(*interval.a);
		b = new point(*interval.b);
		return *this;
	}

	template<int PRECISION> 
	point proper_interval<PRECISION>::width()
	{
		ROUND_UP;
		point ret = *b - *a;
		ROUND_NEAREST;
		return ret;
	}

	template<int PRECISION> const proper_interval<PRECISION> proper_interval<PRECISION>::operator +(const proper_interval<PRECISION>& inter)
	{
		proper_interval<PRECISION> ret;
		ROUND_DOWN;
		*(ret.a) = *(this->a) + *(inter.a);
		ROUND_UP;
		*(ret.b) = *(this->b) + *(inter.b);
		ROUND_NEAREST;
		return ret;
	}

	template<int PRECISION> const proper_interval<PRECISION> proper_interval<PRECISION>::operator -(const proper_interval<PRECISION>& inter)
	{
		proper_interval<PRECISION> ret;
		ROUND_DOWN;
		*(ret.a)=*(this->a) - *(inter.b);
		ROUND_UP;
		*(ret.b) = *(this->b) - *(inter.a);
		ROUND_NEAREST;
		return ret;
	}

	template<int PRECISION> const proper_interval<PRECISION> proper_interval<PRECISION>::operator *(const proper_interval<PRECISION>& inter)
	{
		mpfr::mpreal::set_default_prec(mpfr::digits2bits(PRECISION));
		
		point tmp[4];
		proper_interval<PRECISION> ret;
		
		ROUND_DOWN;
		tmp[0] = *(this->a) * *(inter.a);
		tmp[1] = *(this->a) * *(inter.b);
		tmp[2] = *(this->b) * *(inter.a);
		tmp[3] = *(this->b) * *(inter.b);
		
		*(ret.a) = std::min(std::min(tmp[0],tmp[1]),std::min(tmp[2],tmp[3]));
		ROUND_UP;
		
		tmp[0] = *(this->a) * *(inter.a);
		tmp[1] = *(this->a) * *(inter.b);
		tmp[2] = *(this->b) * *(inter.a);
		tmp[3] = *(this->b) * *(inter.b);
		
		*(ret.b) = std::max(std::max(tmp[0],tmp[1]),std::max(tmp[2],tmp[3]));
		ROUND_NEAREST;
		return ret;
	}

	template<int PRECISION> const proper_interval<PRECISION> proper_interval<PRECISION>::operator /(const proper_interval<PRECISION>& inter)
	{
		if( *(inter.a) <=0 && *b >= 0) throw dividing_exception<PRECISION>(inter);
		mpfr::mpreal::set_default_prec(mpfr::digits2bits(PRECISION));
		
		point tmp[4];
		proper_interval<PRECISION> ret;
		
		ROUND_DOWN;
		tmp[0] = *(this->a) / *(inter.a);
		tmp[1] = *(this->a) / *(inter.b);
		tmp[2] = *(this->b) / *(inter.a);
		tmp[3] = *(this->b) / *(inter.b);
		
		*(ret.a) =std::min(std::min(tmp[0],tmp[1]),std::min(tmp[2],tmp[3]));
		ROUND_UP;
		
		tmp[0] = *(this->a) / *(inter.a);
		tmp[1] = *(this->a) / *(inter.b);
		tmp[2] = *(this->b) / *(inter.a);
		tmp[3] = *(this->b) / *(inter.b);
		
		*(ret.b)=std::max(std::max(tmp[0],tmp[1]),std::max(tmp[2],tmp[3]));
		ROUND_NEAREST;
		return ret;
	}

	template<int PRECISION> proper_interval<PRECISION>& proper_interval<PRECISION>::operator +=(const proper_interval<PRECISION>& inter)
	{
		*this = *this + inter;
		return *this;
	}

	template<int PRECISION> proper_interval<PRECISION>& proper_interval<PRECISION>::operator -=(const proper_interval<PRECISION>& inter)
	{
		*this = *this - inter;
		return *this;
	}

	template<int PRECISION> proper_interval<PRECISION>& proper_interval<PRECISION>::operator *=(const proper_interval<PRECISION>& inter)
	{
		*this = *this * inter;
		return *this;
	}

	template<int PRECISION> proper_interval<PRECISION>& proper_interval<PRECISION>::operator /=(const proper_interval<PRECISION>& inter)
	{
		*this = *this / inter;
		return *this;
	}

	template<int PRECISION> std::ostream& operator<<(std::ostream& os , proper_interval<PRECISION>& inter)
	{
		//"%1.17RDe" "%1.17RUe"
		return os<<"["<<(inter.a)->toString("%1.17Re")<<"; "<<(inter.b)->toString("%1.17Re")<<"], d="<<inter.width().toString("%1.1RUe");
	}

	
	/*!
	 * Some typedefs
	 */
	typedef proper_interval<8> proper_interval_8;
	typedef proper_interval<10> proper_interval_10;
	typedef proper_interval<16> proper_interval_16;
}

#endif
