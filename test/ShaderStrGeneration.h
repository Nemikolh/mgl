#ifndef SHADERSTRGENERATION_H_
#define SHADERSTRGENERATION_H_

#include <cxxtest/TestSuite.h>

#include "../src/core/gl/shader/glsh_var.hpp"

using namespace nkh::core::gl::glsh;

class ShaderStrGeneration : public CxxTest::TestSuite
{

public:
	void setUp()
	{
	}

	void tearDown()
	{
	}

	void test_get_body()
	{
        var<int> a("a"), b("b");
	    a = a + b;
	    TS_ASSERT_EQUALS(a.get_body(), "a = (a + b);\n")
	    b = a;
	    TS_ASSERT_EQUALS(b.get_body(), "a = (a + b);\nb = a;\n")
	}

};

#endif /*SHADERSTRGENERATION_H_*/
