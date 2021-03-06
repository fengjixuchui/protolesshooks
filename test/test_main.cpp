#include "plh_Hook.h"
#include <stdio.h>

#undef NDEBUG
#include <assert.h>

//..............................................................................

namespace test_params {

// target function -- all register arguments are used up

int foo(
	int a, double b,
	int c, double d,
	int e, double f,  // on microsoft x64, e passed on stack
	int g, double h,
	int i, double j,
	int k, double l,
	int m, double n,  // on systemv amd64, m passed on stack
	int o, double p,
	int q, double r   // on systemv amd64, r passed on stack
	)
{
	printf(
		"foo("
		"%d, %f, %d, %f, %d, %f, "
		"%d, %f, %d, %f, %d, %f, "
		"%d, %f, %d, %f, %d, %f)\n",
		a, b, c, d, e, f,
		g, h, i, j, k, l,
		m, n, o, p, q, r
		);

	assert(
		a == 1 && b == 10.1 &&
		c == 2 && d == 20.2 &&
		e == 3 && f == 30.3 &&
		g == 4 && h == 40.4 &&
		i == 5 && j == 50.5 &&
		k == 6 && l == 60.6 &&
		m == 7 && n == 70.7 &&
		o == 8 && p == 80.8 &&
		q == 9 && r == 90.9
		);

	return 123;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

plh::HookAction
fooHookEnter(
	void* targetFunc,
	void* callbackParam,
	size_t frameBase
	)
{
	printf(
		"fooHookEnter(func: %p, param: %p, frame: %p)\n",
		targetFunc,
		callbackParam,
		(void*)frameBase
		);

#if (_PLH_CPU_AMD64)
#	if (_PLH_CPP_MSC)
	plh::RegArgBlock* regArgBlock = (plh::RegArgBlock*)(frameBase + plh::FrameOffset_RegArgBlock);

	int a    = (int)regArgBlock->m_rcx;
	double b = regArgBlock->m_xmm1[0];
	int c    = (int)regArgBlock->m_r8;
	double d = regArgBlock->m_xmm3[0];

	plh::VaList va;
	plh::vaStart(va, frameBase);

	int e = plh::vaArg<int>(va);
	double f = plh::vaArg<double>(va);
	int g = plh::vaArg<int>(va);
	double h = plh::vaArg<double>(va);
	int i = plh::vaArg<int>(va);
	double j = plh::vaArg<double>(va);
	int k = plh::vaArg<int>(va);
	double l = plh::vaArg<double>(va);
	int m = plh::vaArg<int>(va);
	double n = plh::vaArg<double>(va);
	int o = plh::vaArg<int>(va);
	double p = plh::vaArg<double>(va);
	int q = plh::vaArg<int>(va);
	double r = plh::vaArg<double>(va);

#	elif (_PLH_CPP_GCC)
	plh::RegArgBlock* regArgBlock = (plh::RegArgBlock*)(frameBase + plh::FrameOffset_RegArgBlock);

	int a = (int)regArgBlock->m_rdi;
	int c = (int)regArgBlock->m_rsi;
	int e = (int)regArgBlock->m_rdx;
	int g = (int)regArgBlock->m_rcx;
	int i = (int)regArgBlock->m_r8;
	int k = (int)regArgBlock->m_r9;
	double b = regArgBlock->m_xmm0[0];
	double d = regArgBlock->m_xmm1[0];
	double f = regArgBlock->m_xmm2[0];
	double h = regArgBlock->m_xmm3[0];
	double j = regArgBlock->m_xmm4[0];
	double l = regArgBlock->m_xmm5[0];
	double n = regArgBlock->m_xmm6[0];
	double p = regArgBlock->m_xmm7[0];

	plh::VaList va;
	plh::vaStart(va, frameBase);

	int m = plh::vaArg<int>(va);
	int o = plh::vaArg<int>(va);
	int q = plh::vaArg<int>(va);
	double r = plh::vaArg<double>(va);

#	endif
#elif (_PLH_CPU_X86)
	plh::VaList va;
	plh::vaStart(va, frameBase);

	int a = plh::vaArg<int>(va);
	double b = plh::vaArg<double>(va);
	int c = plh::vaArg<int>(va);
	double d = plh::vaArg<double>(va);
	int e = plh::vaArg<int>(va);
	double f = plh::vaArg<double>(va);
	int g = plh::vaArg<int>(va);
	double h = plh::vaArg<double>(va);
	int i = plh::vaArg<int>(va);
	double j = plh::vaArg<double>(va);
	int k = plh::vaArg<int>(va);
	double l = plh::vaArg<double>(va);
	int m = plh::vaArg<int>(va);
	double n = plh::vaArg<double>(va);
	int o = plh::vaArg<int>(va);
	double p = plh::vaArg<double>(va);
	int q = plh::vaArg<int>(va);
	double r = plh::vaArg<double>(va);

#endif

	printf(
		"  ("
		"%d, %f, %d, %f, %d, %f, "
		"%d, %f, %d, %f, %d, %f, "
		"%d, %f, %d, %f, %d, %f)\n",
		a, b, c, d, e, f,
		g, h, i, j, k, l,
		m, n, o, p, q, r
		);

	assert(
		a == 1 && b == 10.1 &&
		c == 2 && d == 20.2 &&
		e == 3 && f == 30.3 &&
		g == 4 && h == 40.4 &&
		i == 5 && j == 50.5 &&
		k == 6 && l == 60.6 &&
		m == 7 && n == 70.7 &&
		o == 8 && p == 80.8 &&
		q == 9 && r == 90.9
		);

	return plh::HookAction_Default;
}

void
fooHookLeave(
	void* targetFunc,
	void* callbackParam,
	size_t frameBase
	)
{
	plh::RegRetBlock* regRetBlock = (plh::RegRetBlock*)(frameBase + plh::FrameOffset_RegRetBlock);

#if (_PLH_CPU_AMD64)
	int returnValue = (int)regRetBlock->m_rax;
#elif (_PLH_CPU_X86)
	int returnValue = regRetBlock->m_eax;
#endif

	printf(
		"fooHookLeave(func: %p, param: %p, frame: %p, retval: %d/0x%x)\n",
		targetFunc,
		callbackParam,
		(void*)frameBase,
		returnValue,
		returnValue
		);
}

} // namespace test_params

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
testParams()
{
	using namespace test_params;

	typedef int FooFunc(
		int, double,
		int, double,
		int, double,
		int, double,
		int, double,
		int, double,
		int, double,
		int, double,
		int, double
		);

	plh::HookArena arena;
	plh::Hook* fooHook = arena.allocate((void*)foo, (void*)0xabcdef, fooHookEnter, fooHookLeave);
	plh::enableHooks();

	((FooFunc*)fooHook)(
		1, 10.1,
		2, 20.2,
		3, 30.3,
		4, 40.4,
		5, 50.5,
		6, 60.6,
		7, 70.7,
		8, 80.8,
		9, 90.9
		);
}

//..............................................................................

namespace test_modify {

// target function

int foo(int a)
{
	printf("foo(%d)\n", a);
	assert(a == 20);
	return 123;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

plh::HookAction
fooHookEnter(
	void* targetFunc,
	void* callbackParam,
	size_t frameBase
	)
{
	printf(
		"fooHookEnter(func: %p, param: %p, frame: %p)\n",
		targetFunc,
		callbackParam,
		(void*)frameBase
		);

#if (_PLH_CPU_AMD64)
#	if (_PLH_CPP_MSC)
	plh::RegArgBlock* regArgBlock = (plh::RegArgBlock*)(frameBase + plh::FrameOffset_RegArgBlock);
	int a = (int)regArgBlock->m_rcx;
	int newA = a * 2;
	regArgBlock->m_rcx = newA;
#	elif (_PLH_CPP_GCC)
	plh::RegArgBlock* regArgBlock = (plh::RegArgBlock*)(frameBase + plh::FrameOffset_RegArgBlock);
	int a = (int)regArgBlock->m_rdi;
	int newA = a * 2;
	regArgBlock->m_rdi = newA;
#	endif
#elif (_PLH_CPU_X86)
	plh::VaList va;
	plh::vaStart(va, frameBase);
	int* p = &plh::vaArg<int>(va);
	int a = *p;
	int newA = a * 2;
	*p = newA;
#endif

	printf("  modifying arg: (%d -> %d)\n", a, newA);
	assert(a == 10);
	return plh::HookAction_Default;
}

void
fooHookLeave(
	void* targetFunc,
	void* callbackParam,
	size_t frameBase
	)
{
	printf(
		"fooHookLeave(func: %p, param: %p, frame: %p)\n",
		targetFunc,
		callbackParam,
		(void*)frameBase
		);

	plh::RegRetBlock* regRetBlock = (plh::RegRetBlock*)(frameBase + plh::FrameOffset_RegRetBlock);

#if (_PLH_CPU_AMD64)
	int returnValue = (int)regRetBlock->m_rax;
	int newReturnValue = returnValue * 2;
	regRetBlock->m_rax = newReturnValue;
#elif (_PLH_CPU_X86)
	int returnValue = regRetBlock->m_eax;
	int newReturnValue = returnValue * 2;
	regRetBlock->m_eax = newReturnValue;
#endif

	printf(
		"  modifying retval: %d -> %d\n",
		returnValue,
		newReturnValue
		);

	assert(returnValue == 123);
}

} // namespace test_modify

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
testModify()
{
	using namespace test_modify;

	typedef int FooFunc(int);

	plh::HookArena arena;
	plh::Hook* fooHook = arena.allocate((void*)foo, (void*)0xabcdef, fooHookEnter, fooHookLeave);
	plh::enableHooks();
	int result = ((FooFunc*)fooHook)(10);
	printf("result: %d\n", result);
	assert(result == 246);
}

//..............................................................................

namespace test_block {

// target function

int foo(int x)
{
	printf("foo(%d)\n", x);
	return 123;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// enter hook

plh::HookAction
fooHookEnter(
	void* targetFunc,
	void* callbackParam,
	size_t frameBase
	)
{
	printf(
		"fooHookEnter(func: %p, param: %p, frame: %p)\n",
		targetFunc,
		callbackParam,
		(void*)frameBase
		);

#if (_PLH_CPU_AMD64)
#	if (_PLH_CPP_MSC)
	plh::RegArgBlock* regArgBlock = (plh::RegArgBlock*)(frameBase + plh::FrameOffset_RegArgBlock);
	int a = (int)regArgBlock->m_rcx;
#	elif (_PLH_CPP_GCC)
	plh::RegArgBlock* regArgBlock = (plh::RegArgBlock*)(frameBase + plh::FrameOffset_RegArgBlock);
	int a = (int)regArgBlock->m_rdi;
#	endif
#elif (_PLH_CPU_X86)
	plh::VaList va;
	plh::vaStart(va, frameBase);
	int a = plh::vaArg<int>(va);
#endif

	plh::RegRetBlock* regRetBlock = (plh::RegRetBlock*)(frameBase + plh::FrameOffset_RegRetBlock);

	int returnValue;

	switch (a)
	{
	case 0:
		return plh::HookAction_JumpTarget; // in this case, it's the same as HookAction_Default

	case 1:
		returnValue = foo(a);
		break;

	case 2:
		returnValue = 246;
		break;

	default:
		assert(false);
	}

#if (_PLH_CPU_AMD64)
	regRetBlock->m_rax = returnValue;
#elif (_PLH_CPU_X86)
	regRetBlock->m_eax = returnValue;
#endif

	return plh::HookAction_Return;
}

} // namespace test_block

void
testBlock()
{
	using namespace test_block;

	typedef int FooFunc(int);

	plh::HookArena arena;
	plh::Hook* fooHook = arena.allocate((void*)foo, (void*)0xabcdef, fooHookEnter, NULL);
	plh::enableHooks();

	printf("pass-through...\n");
	int result = ((FooFunc*)fooHook)(0);
	printf("result: %d\n", result);
	assert(result == 123);

	printf("proxy-call...\n");
	result = ((FooFunc*)fooHook)(1);
	printf("result: %d\n", result);
	assert(result == 123);

	printf("block completely...\n");
	result = ((FooFunc*)fooHook)(2);
	printf("result: %d\n", result);
	assert(result == 246);
}

//..............................................................................

int
main()
{
	testParams();
	testModify();
	testBlock();
	return 0;
}

//..............................................................................
