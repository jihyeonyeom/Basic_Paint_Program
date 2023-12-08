
// Basic_Paint_ProgramView.h: CBasicPaintProgramView 클래스의 인터페이스
//

#pragma once
#include "CMyShape.h"
#include<vector>

class CBasicPaintProgramView : public CView
{
protected: // serialization에서만 만들어집니다.
	CBasicPaintProgramView() noexcept;
	DECLARE_DYNCREATE(CBasicPaintProgramView)

// 특성입니다.
public:
	CBasicPaintProgramDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CBasicPaintProgramView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

private:
	std::vector<CMyShape> m_shapeVector; // 그린 객체 저장
	CMyShape* m_shape; // 현재 그리는 객체
	ShapeType m_shapeType; // 현재 그리기 타입
	Color m_lineColor; // 현재 선 색상
	Color m_fillColor; // 현재 채우기 색상
	float m_thickness; // 현재 선 두께
	BOOL m_bDrawing; // 마우스 왼쪽 버튼이 클릭된 상태 여부

	bool m_selectShape; // 선택된 객체를 나타내는 포인터
	size_t m_selectIdx; // 선택된 객체의 벡터 내 인덱스 번호
	
	bool m_selectMove; // 선택된 객체의 이동 여부
	CPoint m_selectMovePoint; // 선택된 객체의 이동 위치

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDrawLine();
	afx_msg void OnDrawRectangle();
	afx_msg void OnDrawCircle();

	// 선 색상
	afx_msg void OnLineBlack();
	afx_msg void OnLineRed();
	afx_msg void OnLineGreen();
	afx_msg void OnLineBlue();

	// 채우기 색상
	afx_msg void OnFillBlack();
	afx_msg void OnFillRed();
	afx_msg void OnFillGreen();
	afx_msg void OnFillBlue();
	afx_msg void OnFillWhite();

	// 선 두께
	afx_msg void OnThickness1();
	afx_msg void OnThickness2();
	afx_msg void OnThickness3();
	afx_msg void OnSelectObject();
	afx_msg void OnSelectDelete();
};

#ifndef _DEBUG  // Basic_Paint_ProgramView.cpp의 디버그 버전
inline CBasicPaintProgramDoc* CBasicPaintProgramView::GetDocument() const
   { return reinterpret_cast<CBasicPaintProgramDoc*>(m_pDocument); }
#endif

