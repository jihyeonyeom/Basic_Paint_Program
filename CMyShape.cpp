// CMyShape.cpp: 구현 파일
//

#include "pch.h"
#include "Basic_Paint_Program.h"
#include "CMyShape.h"

// CMyShape

IMPLEMENT_DYNAMIC(CMyShape, CWnd)

// CMyShape 생성자
CMyShape::CMyShape(ShapeType type, CPoint startPoint, CPoint endPoint, Color lineColor, Color fillColor, float thickness)
	: type(type), startPoint(startPoint), endPoint(endPoint), lineColor(lineColor), fillColor(fillColor), thickness(thickness){}

// CMyShape 소멸자
CMyShape::~CMyShape()
{
}

// 복사 생성자 정의
CMyShape::CMyShape(const CMyShape& other)
	: type(other.type), startPoint(other.startPoint), endPoint(other.endPoint), lineColor(other.lineColor), fillColor(other.fillColor), thickness(other.thickness){}

// 대입 연산자 정의
CMyShape& CMyShape::operator=(const CMyShape& other) {
	if (this != &other) {
		type = other.type;
		startPoint = other.startPoint;
		endPoint = other.endPoint;
		lineColor = other.lineColor;
		fillColor = other.fillColor;
		thickness = other.thickness;
	}
	return *this;
}

// --- 멤버 함수 ---

// EndPoint 설정 함수
void CMyShape::SetEndPoint(CPoint endPoint) {
	this->endPoint = endPoint;
}

// 그리기 함수
void CMyShape::Draw(Graphics* graphics, bool selected) const {
	// GDI+ Brush 설정
	SolidBrush sBrush(fillColor);

	// GDI+ Pen 설정
	Pen pen(lineColor, thickness);
	// 선택된 객체인 경우 점선 GDI+ Pen 설정
	if (selected) {
		pen.SetColor(Color(255, 0, 0, 0));
		pen.SetWidth(2.0f);
		pen.SetDashStyle(DashStyleDash);
	}

	// GDI+ 사각형, 원 위치, 너비
	int startX = min(startPoint.x, endPoint.x);
	int startY = min(startPoint.y, endPoint.y);
	int width = max(startPoint.x, endPoint.x) - startX;
	int height = max(startPoint.y, endPoint.y) - startY;

	switch (type) {
	case LINE:
		graphics->DrawLine(&pen, startPoint.x, startPoint.y, endPoint.x, endPoint.y);
		break;
	case RECTANGLE:
		graphics->FillRectangle(&sBrush, startX, startY, width, height);
		graphics->DrawRectangle(&pen, startX, startY, width, height);
		break;
	case CIRCLE:
		graphics->FillEllipse(&sBrush, startX, startY, width, height);
		graphics->DrawEllipse(&pen, startX, startY, width, height);
		break;
	}
}

// 해당 좌표가 객체 내에 있는지 확인하는 함수
bool CMyShape::IsPointInside(CPoint point) const {
	LONG startX = min(startPoint.x, endPoint.x);
	LONG startY = min(startPoint.y, endPoint.y);
	LONG endX = max(startPoint.x, endPoint.x);
	LONG endY = max(startPoint.y, endPoint.y);

	if (type == LINE) {
		// 점과 직선 사이의 거리 공식 (픽셀 거리)
		double distance = abs((endY - startY) * point.x - (endX - startX) * point.y + endX * startY - endY * startX) / sqrt(pow(endY - startY, 2) + pow(endX - startX, 2));
		return distance <= 5.0;
	}
	else if (type == CIRCLE) {
		// 원의 중심 좌표와 각 축에 대한 반지름 정보를 얻어옴
		LONG centerX = (startPoint.x + endPoint.x) / 2;
		LONG centerY = (startPoint.y + endPoint.y) / 2;
		double radiusX = abs(endPoint.x - centerX);
		double radiusY = abs(endPoint.y - centerY);

		// 주어진 점과 각 축에 대한 거리 계산
		double distanceToCenterX = abs(point.x - centerX);
		double distanceToCenterY = abs(point.y - centerY);

		// 점이 타원 안에 있는지 여부를 반환
		return (pow(distanceToCenterX / radiusX, 2) + pow(distanceToCenterY / radiusY, 2)) <= 1;
	}
	else if (type == RECTANGLE) return ((point.x >= startX && point.x <= endX) && (point.y >= startY && point.y <= endY));
}

// 해당 도형 전체 이동 함수
void CMyShape::ObjectMove(CPoint point) {
	startPoint.x += point.x;
	startPoint.y += point.y;
	endPoint.x += point.x;
	endPoint.y += point.y;
}

BEGIN_MESSAGE_MAP(CMyShape, CWnd)
END_MESSAGE_MAP()

// CMyShape 메시지 처리기