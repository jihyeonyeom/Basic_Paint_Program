
// Basic_Paint_ProgramView.cpp: CBasicPaintProgramView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Basic_Paint_Program.h"
#endif

#include "Basic_Paint_ProgramDoc.h"
#include "Basic_Paint_ProgramView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBasicPaintProgramView

IMPLEMENT_DYNCREATE(CBasicPaintProgramView, CView)

BEGIN_MESSAGE_MAP(CBasicPaintProgramView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_32771, &CBasicPaintProgramView::OnDrawLine)
	ON_COMMAND(ID_32772, &CBasicPaintProgramView::OnDrawRectangle)
	ON_COMMAND(ID_32773, &CBasicPaintProgramView::OnDrawCircle)
	ON_COMMAND(ID_LINE_BLACK, &CBasicPaintProgramView::OnLineBlack)
	ON_COMMAND(ID_LINE_RED, &CBasicPaintProgramView::OnLineRed)
	ON_COMMAND(ID_LINE_GREEN, &CBasicPaintProgramView::OnLineGreen)
	ON_COMMAND(ID_LINE_BLUE, &CBasicPaintProgramView::OnLineBlue)
	ON_COMMAND(ID_FILL_BLACK, &CBasicPaintProgramView::OnFillBlack)
	ON_COMMAND(ID_FILL_RED, &CBasicPaintProgramView::OnFillRed)
	ON_COMMAND(ID_FILL_GREEN, &CBasicPaintProgramView::OnFillGreen)
	ON_COMMAND(ID_FILL_BLUE, &CBasicPaintProgramView::OnFillBlue)
	ON_COMMAND(ID_FILL_WHITE, &CBasicPaintProgramView::OnFillWhite)
	ON_COMMAND(ID_THICKNESS_1, &CBasicPaintProgramView::OnThickness1)
	ON_COMMAND(ID_THICKNESS_2, &CBasicPaintProgramView::OnThickness2)
	ON_COMMAND(ID_THICKNESS_3, &CBasicPaintProgramView::OnThickness3)
	ON_COMMAND(ID_SELECT_OBJECT, &CBasicPaintProgramView::OnSelectObject)
	ON_COMMAND(ID_SELECT_DELETE, &CBasicPaintProgramView::OnSelectDelete)
END_MESSAGE_MAP()

// CBasicPaintProgramView 생성/소멸

CBasicPaintProgramView::CBasicPaintProgramView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_shapeType = EMPTY;
	m_lineColor = Color(255, 0, 0, 0);
	m_fillColor = Color(128, 255, 255, 255);
	m_thickness = 1.0f;
	m_shape = new CMyShape(m_shapeType, CPoint(0, 0), CPoint(0, 0), m_lineColor, m_fillColor, m_thickness);
	m_selectShape = FALSE;
	m_selectIdx = -1;
	m_bDrawing = FALSE;

	m_selectMove = FALSE;
	m_selectMovePoint = (-1, -1);
}

CBasicPaintProgramView::~CBasicPaintProgramView()
{
}

BOOL CBasicPaintProgramView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CBasicPaintProgramView 그리기

void CBasicPaintProgramView::OnDraw(CDC* pDC)
{
	CBasicPaintProgramDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	
	// 현재 윈도우의 크기와 위치를 나타내는 cRect 객체 생성
	CRect cRect;
	GetClientRect(&cRect);

	// GDI+의 cRect를 기반으로 Rect 객체를 생성
	Rect gdiRect(cRect.left, cRect.top, cRect.Width(), cRect.Height());

	// pDC와 호환되는 메모리 디바이스 컨텍스트 생성
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);

	// pDC와 호환 가능한 CBitmap 객체 생성
	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, cRect.Width(), cRect.Height());

	// pDC와 호환된 핸들로 변경
	CBitmap* pOldBitmap = MemDC.SelectObject(&memBitmap);

	// 메모리 디바이스 컨텍스트의 핸들을 매개변수로 GDI+ mem 객체 생성
	Graphics mem(MemDC.m_hDC);
	mem.SetSmoothingMode(SmoothingModeHighQuality); // 부드럽게 그리기

	SolidBrush sBrush(Color(255, 255, 255, 255));
	mem.FillRectangle(&sBrush, gdiRect);

	// 기존 객체 그리기
	for (size_t i = 0; i < m_shapeVector.size(); i++) {
		if (m_selectIdx == i) m_shapeVector[i].Draw(&mem, m_selectShape);
		else m_shapeVector[i].Draw(&mem);
	}

	// 새로운 드래그 객체 그리기
	if (m_bDrawing) {
		m_shape->Draw(&mem, FALSE);
	}

	// MemDC에 그려진 그림을 화면에 복사
	pDC->BitBlt(0, 0, cRect.right, cRect.bottom, &MemDC, 0, 0, SRCCOPY);

	// 리소스 해제
	MemDC.SelectObject(pOldBitmap);
	mem.ReleaseHDC(pDC->m_hDC);
}


// CBasicPaintProgramView 인쇄

BOOL CBasicPaintProgramView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CBasicPaintProgramView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CBasicPaintProgramView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CBasicPaintProgramView 진단

#ifdef _DEBUG
void CBasicPaintProgramView::AssertValid() const
{
	CView::AssertValid();
}

void CBasicPaintProgramView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBasicPaintProgramDoc* CBasicPaintProgramView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBasicPaintProgramDoc)));
	return (CBasicPaintProgramDoc*)m_pDocument;
}
#endif //_DEBUG


// CBasicPaintProgramView 메시지 처리기

BOOL CBasicPaintProgramView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return TRUE;
	//return CView::OnEraseBkgnd(pDC);
}


void CBasicPaintProgramView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CView::OnLButtonDown(nFlags, point);

	// 도형 그리기
	if (!m_bDrawing && (m_shapeType == LINE || m_shapeType == RECTANGLE || m_shapeType == CIRCLE)) {
		delete m_shape;
		m_bDrawing = TRUE;
		m_shape = new CMyShape(m_shapeType, point, point, m_lineColor, m_fillColor, m_thickness);
	}
	// 도형 선택
	else if (!m_bDrawing && m_shapeType == SELECT && m_shapeVector.size() > 0) {
		for (size_t i = m_shapeVector.size(); i != 0; --i) {
			if (m_shapeVector[i - 1].IsPointInside(point)) {
				// 도형 선택
				m_selectShape = TRUE;
				m_selectIdx = i - 1;

				// 도형 이동
				m_selectMove = TRUE;
				m_selectMovePoint = point;
				break;
			}
			else {
				// 초기화
				m_selectShape = FALSE;
				m_selectIdx = -1;
				m_selectMove = FALSE;
			}
		}
		Invalidate();
	}
}

void CBasicPaintProgramView::OnMouseMove(UINT nFlags, CPoint point)
{
	CView::OnMouseMove(nFlags, point);

	// 도형 그리기
	if (m_bDrawing) {
		m_shape->SetEndPoint(point);
		Invalidate();
	}
	// 선택 도형 이동
	else if (m_selectShape && m_selectMove) {
		m_selectMovePoint.x = point.x - m_selectMovePoint.x;
		m_selectMovePoint.y = point.y - m_selectMovePoint.y;
		m_shapeVector[m_selectIdx].ObjectMove(m_selectMovePoint);
		m_selectMovePoint = point;
		Invalidate();
	}
}

void CBasicPaintProgramView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CView::OnLButtonUp(nFlags, point);

	// 도형 그리기
	if (m_bDrawing) { 
		m_bDrawing = FALSE;
		m_shape->SetEndPoint(point);
		m_shapeVector.push_back(*m_shape);
		delete m_shape;
		m_shape = nullptr;
		Invalidate();
	}
	// 선택 도형 이동
	else if (m_selectShape && m_selectMove) {
		m_selectMove = FALSE;
		Invalidate();
	}
}

// ----------- 그리기 모양 ---------------
void CBasicPaintProgramView::OnDrawLine()
{
	m_shapeType = LINE;

	// 선택 객체 초기화
	m_selectShape = FALSE;
	m_selectIdx = -1;
	Invalidate();
}

void CBasicPaintProgramView::OnDrawRectangle()
{
	m_shapeType = RECTANGLE;

	// 선택 객체 초기화
	m_selectShape = FALSE;
	m_selectIdx = -1;
	Invalidate();
}

void CBasicPaintProgramView::OnDrawCircle()
{
	m_shapeType = CIRCLE;

	// 선택 객체 초기화
	m_selectShape = FALSE;
	m_selectIdx = -1;
	Invalidate();
}
// ---------------------------------------

// ------------- 선 색상 -----------------
void CBasicPaintProgramView::OnLineBlack()
{
	m_lineColor = Color(255, 0, 0, 0);
}

void CBasicPaintProgramView::OnLineRed()
{
	m_lineColor = Color(255, 255, 0, 0);
}

void CBasicPaintProgramView::OnLineGreen()
{
	m_lineColor = Color(255, 0, 255, 0);
}

void CBasicPaintProgramView::OnLineBlue()
{
	m_lineColor = Color(255, 0, 0, 255);
}
// ---------------------------------------

// ------------ 채우기 색상 --------------
void CBasicPaintProgramView::OnFillBlack()
{
	m_fillColor = Color(128, 0, 0, 0);
}

void CBasicPaintProgramView::OnFillRed()
{
	m_fillColor = Color(128, 255, 0, 0);
}

void CBasicPaintProgramView::OnFillGreen()
{
	m_fillColor = Color(128, 0, 255, 0);
}

void CBasicPaintProgramView::OnFillBlue()
{
	m_fillColor = Color(128, 0, 0, 255);
}

void CBasicPaintProgramView::OnFillWhite()
{
	m_fillColor = Color(128, 255, 255, 255);
}
// ----------------------------------------

// -------------- 선 두께 -----------------
void CBasicPaintProgramView::OnThickness1()
{
	m_thickness = 1.0f;
}

void CBasicPaintProgramView::OnThickness2()
{
	m_thickness = 2.0f;
}

void CBasicPaintProgramView::OnThickness3()
{
	m_thickness = 3.0f;
}
// ----------------------------------------

// -------------- 도형 선택 ---------------
void CBasicPaintProgramView::OnSelectObject()
{
	m_shapeType = SELECT;
}
// ----------------------------------------

// -------------- 도형 삭제 ---------------
void CBasicPaintProgramView::OnSelectDelete()
{
	if (m_selectShape) {
		m_shapeVector.erase(m_shapeVector.begin() + m_selectIdx);
		m_selectShape = FALSE;
		m_selectIdx = -1;
		Invalidate();
	}
}
// ----------------------------------------