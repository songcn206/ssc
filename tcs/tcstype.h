#ifndef __tcstype_h
#define __tcstype_h

#define TCS_KERNEL_VERSION 1

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1
#endif

#ifdef __cplusplus
extern "C" {
#endif

	enum { 
		/* variable types */
		TCS_INVALID, TCS_INPUT, TCS_PARAM, TCS_OUTPUT, TCS_DEBUG, 
		/* data types */
		TCS_NUMBER, TCS_ARRAY, TCS_MATRIX, TCS_STRING };

	struct _tcsvalue {
		unsigned char type;
		union {
			double value;
			struct {
				double *values;
				unsigned int length;
			} array;
			struct {
				double *values;
				unsigned int nrows, ncols;
			} matrix;
			char *cstr;			
		} data;
	};
	typedef struct _tcsvalue tcsvalue;

#define TCS_MATRIX_INDEX( v, r, c )  ((v)->data.matrix.values[ (v)->data.matrix.ncols * (r) + (c) ])

	enum { TCS_NOTICE, TCS_WARNING, TCS_ERROR }; // message types

	struct _tcscontext {
		void *kernel_internal;
		int unit_internal;
		void (*message) ( struct _tcscontext *cxt, int msgtype, const char *message );
		bool (*progress) ( struct _tcscontext *cxt, float percent, const char *status );
		tcsvalue *(*get_value)( struct _tcscontext *cxt, int idx );
		int (*get_num_values)( struct _tcscontext *cxt );
		void (*tcsvalue_set_number)( tcsvalue *, double );
		void (*tcsvalue_set_array)( tcsvalue *, double *, int );
		void (*tcsvalue_set_matrix)( tcsvalue *, double *, int, int );
		void (*tcsvalue_set_string)( tcsvalue *, const char * );
	};
	typedef struct _tcscontext tcscontext;
	
	
	struct _tcsvarinfo {
		unsigned char var_type; // (TCS_INVALID)   TCS_INPUT, TCS_OUTPUT, TCS_PARAM, TCS_DEBUG
		unsigned char data_type; // (TCS_INVALID)   TCS_NUMBER, TCS_ARRAY, TCS_MATRIX, TCS_STRING
		unsigned int index;
		const char *name;
		const char *label;
		const char *units;
		const char *group;
		const char *meta;
		const char *default_value;
	};
	typedef struct _tcsvarinfo tcsvarinfo;

	struct _tcsvisualinfo {
		int min_width;
		int min_height;
		char **pixmap;
		char *conn_left;
		char *conn_right;
	};
	typedef struct _tcsvisualinfo tcsvisualinfo;
	
	enum { TCS_INIT, TCS_INVOKE, TCS_CONVERGED };
	struct _tcstypeinfo	{
		const char *name;
		const char *description;
		const char *author;
		const char *timestamp;
		int version;
		tcsvarinfo *variables;
		tcsvisualinfo *visual;
		int require_kernel_version;
		int call_after_convergence;	
		void *(*create_instance)( tcscontext *cxt, struct _tcstypeinfo *ti );
		void (*free_instance)( void *inst );
		int (*invoke)( tcscontext *cxt, void *inst,
				int ctrl,
				tcsvalue *values, unsigned int nvalues,
				double time,
				double step,
				int ncall );
	};
	typedef struct _tcstypeinfo tcstypeinfo;
	
#ifdef __cplusplus
} // C interface
#endif



#if defined(__cplusplus)
// define easier-to-user C++ interface for type implementation

#include <string>
#include <numeric>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cmath>
#include <limits>
#include <iostream>
#include <algorithm>
#include <sstream>

class tcstypeinterface
{
public:
	tcstypeinterface( tcscontext *cxt, tcstypeinfo *ti )
		: m_context(cxt), m_typeInfo(ti), m_currentTime(0.0), m_timeStep(0.0), m_values(0), m_numValues(0) {  }
	virtual ~tcstypeinterface() {  }

	virtual int init() = 0;
	virtual int call( double time, double step, int ncall ) = 0;
	virtual int converged( double time ) { return 0; }

protected:
	double *allocate( size_t idx, int len, double fill = 0 )
	{
		tcsvalue *v = var(idx);
		if ( v && len > 0 )
		{
			double *p = new double[len];
			for ( int i=0;i<len;i++ ) p[i] = fill;
			m_context->tcsvalue_set_array( v, p, len );
			delete [] p;
			return v->data.array.values;
		}
		return 0;
	}

	double *allocate( size_t idx, int nr, int nc, double fill = 0 )
	{
		tcsvalue *v = var(idx);
		if (v && nr > 0 && nc > 0)
		{
			double *p = new double[nr*nc];
			for ( int i=0;i<nr*nc;i++) p[i] = fill;
			m_context->tcsvalue_set_matrix( v, p, nr, nc );
			delete [] p;
			return v->data.matrix.values;
		}
		return 0;
	}

	inline double current_time() { return m_currentTime; }
	inline double time_step() { return m_timeStep; }
	
	double value( size_t idx )
	{
		tcsvalue *v = var(idx);
		if ( !v || v->type != TCS_NUMBER ) return std::numeric_limits<double>::quiet_NaN();
		return v->data.value;
	}

	void value( size_t idx, double val )
	{
		tcsvalue *v = var(idx);
		if ( !v || v->type != TCS_NUMBER ) return;
		v->data.value = val;
	}
	
	std::string value_str( size_t idx )
	{
		tcsvalue *v = var(idx);
		if ( !v || v->type != TCS_STRING ) return std::string();
		return std::string( v->data.cstr );
	}

	void value_str( size_t idx, const std::string &str )
	{
		tcsvalue *v = var(idx);
		if ( !v || v->type != TCS_STRING ) return;
		m_context->tcsvalue_set_string( v, str.c_str() );
	}

	double *value( size_t idx, int *len )
	{	
		tcsvalue *v = var(idx);
		if (!v || v->type != TCS_ARRAY )
		{
			if (len) *len = 0;
			return 0;
		}
		if (len) *len = v->data.array.length;
		return v->data.array.values;
	}

	double *value( size_t idx, int *nr, int *nc )
	{
		tcsvalue *v = var(idx);
		if ( !v || v->type != TCS_MATRIX )
		{
			if (nr) *nr = 0;
			if (nc) *nc = 0;
			return 0;
		}
		if (nr) *nr = v->data.matrix.nrows;
		if (nc) *nc = v->data.matrix.ncols;
		return v->data.matrix.values;
	}

	void message( int msgtype, const char *fmt, ... )
	{
		char buf[512];
		va_list ap;
		va_start(ap, fmt);
#if defined(_MSC_VER)||defined(_WIN32)
		_vsnprintf(buf, 509, fmt, ap);
#else
		vsnprintf(buf, 509, fmt, ap);
#endif
		va_end(ap);
		m_context->message( m_context, msgtype, buf );
	}

	bool progress( float percent, const char *status )
	{
		return m_context->progress( m_context, percent, status );
	}

	// these calls are always available to access internal unit variable storage
	inline tcsvalue *var( int idx ) { return (m_values != 0 && idx >= 0 && idx < m_numValues) ? &m_values[idx] : m_context->get_value(m_context, idx); }
	inline int nvar() { return (m_values != 0) ? m_numValues : m_context->get_num_values( m_context ); }

private:
	tcscontext *m_context;
	tcstypeinfo *m_typeInfo;
	double m_currentTime;
	double m_timeStep;

	// available as performance enhancement (direct storage access) during iterative calls
	// in general, always use inline'd var() and nvar()
	tcsvalue *m_values;
	int m_numValues;
public:
	inline void __passptr( tcsvalue *v, int n ) { m_values = v; m_numValues = n; }
	inline void __settime( double cur, double step ) { m_currentTime = cur; m_timeStep = step; }
};

#ifdef _TCSTYPEINTERFACE_

static void __free( void *inst )
{
	tcstypeinterface *ti = static_cast<tcstypeinterface*>( inst );
	if ( ti != 0 ) delete ti;
}

static int __invoke( tcscontext *cxt, void *inst,
				int ctrl,
				tcsvalue *values, unsigned int nvalues,
				double time,
				double step,
				int ncall )
{
	tcstypeinterface *ti = static_cast<tcstypeinterface*>( inst );
	if ( ti == 0 )
	{
		cxt->message( cxt, TCS_ERROR, "invalid type invocation: null instance" );
		return -1;
	}

	int code = -999;
	ti->__passptr( values, nvalues );
	ti->__settime( time, step );
	switch( ctrl )
	{
	case TCS_INIT:
		code = ti->init( );
		break;
	case TCS_INVOKE:
		code = ti->call( time, step, ncall );
		break;
	case TCS_CONVERGED:
		code = ti->converged( time );
		break;
	}

	ti->__passptr(0, 0);
	return code;
}

#define TCS_IMPLEMENT_TYPE( mytype, desc_str, author_str, ver_num, vartable, visual, call_post_converge_bool ) \
	static void *__create_ ## mytype( tcscontext *cxt, tcstypeinfo *ti ) { return new mytype( cxt, ti ); } \
	extern "C" { tcstypeinfo __ti_ ## mytype = { #mytype, desc_str, author_str, __DATE__ " " __TIME__, ver_num, vartable, visual, TCS_KERNEL_VERSION, call_post_converge_bool, __create_ ## mytype, __free, __invoke }; }

#endif // _TCSTYPEINTERFACE_

#ifdef _WIN32
#define TCSEXPORT __declspec(dllexport)
#else
#define TCSEXPORT
#endif

#define TCS_DEFINE_TYPE( mytype ) extern "C" { extern tcstypeinfo __ti_ ## mytype; }
#define TCS_BEGIN_EXPORT()        static tcstypeinfo *__exported_types[] = {
#define TCS_EXPORT_TYPE( mytype )     &__ti_ ## mytype,
#define TCS_END_EXPORT()              0 }; extern "C" { \
	TCSEXPORT tcstypeinfo **tcsdynamictypes() { return (tcstypeinfo**)&__exported_types; } }


#endif


#endif

