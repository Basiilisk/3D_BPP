#include "pch.h"
#include "CppUnitTest.h"
#include "../3D_BPP/3D_BPP.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest3DBPP
{
	TEST_CLASS(UnitTest3DBPP)
	{
	public:

		TEST_METHOD(Products_Less_Cases)
		{
			vector<Product> pt1;
			vector<Case> ct1;
			start_norm_rand();
			FillRects(pt1, 5000);
			FillRects(ct1, 10);
			sort_algh_FFDH(pt1, ct1);
			Assert::IsTrue(check_all_cases(pt1, ct1));

		}
		TEST_METHOD(Cases_Less_Products)
		{
			vector<Product> pt2;
			vector<Case> ct2;
			start_norm_rand();
			FillRects(pt2, 10);
			FillRects(ct2, 5000);
			sort_algh_FFDH(pt2, ct2);
			Assert::IsTrue(check_all_cases(pt2, ct2));
		}
		TEST_METHOD(Zero_Products)
		{
			vector<Product> pt3;
			vector<Case> ct3;
			start_norm_rand();
			FillRects(pt3, 0);
			FillRects(ct3, 100);
			sort_algh_FFDH(pt3, ct3);
			Assert::IsTrue(check_all_cases(pt3, ct3));

		}
		TEST_METHOD(Zero_Cases)
		{
			vector<Product> pt4;
			vector<Case> ct4;
			start_norm_rand();
			FillRects(pt4, 10);
			FillRects(ct4, 5000);
			sort_algh_FFDH(pt4, ct4);
			Assert::IsTrue(check_all_cases(pt4, ct4));
		}
	};
}
