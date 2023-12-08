#pragma once


// CMyShape

// 열거형 모양 타입
enum ShapeType {
	LINE,
	RECTANGLE,
	CIRCLE,
	SELECT,
	EMPTY
};

class CMyShape : public CWnd
{
	DECLARE_DYNAMIC(CMyShape)

private:
	ShapeType type; // 그리기 타입
	CPoint startPoint; // 시작 위치
	CPoint endPoint; // 끝 위치
	Color lineColor; // 선 색상
	Color fillColor; // 채우기 색상
	float thickness; // 선 두께

public:
	// 생성자
	CMyShape(ShapeType type, CPoint startPoint, CPoint endPoint, Color lineColor, Color fillColor, float thickness);

	// 소멸자
	virtual ~CMyShape();

	// 복사 생성자 추가
	CMyShape(const CMyShape& other);

	// 대입 연산자 추가
	CMyShape& operator=(const CMyShape& other);

	//--- 멤버 함수 ---
	
	// EndPoint 설정 함수
	void SetEndPoint(CPoint endPoint);

	// 그리기 함수
	void Draw(Graphics* g, bool selected = FALSE) const;

	// 해당 좌표가 객체 내에 있는지 확인하는 함수
	bool IsPointInside(CPoint point) const;

	// 해당 도형 전체 이동 함수
	void ObjectMove(CPoint point);

protected:
	DECLARE_MESSAGE_MAP()
};


