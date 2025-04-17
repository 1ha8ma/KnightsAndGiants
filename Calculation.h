#pragma once

class Calculation
{
public:
	Calculation() {};
	~Calculation() {};

	//線形補間
	VECTOR Lerp(VECTOR targetPos, VECTOR nowPos, float speed, float& t);

	//ロドリゲスの回転公式
	VECTOR RodriguesRotationFormula(VECTOR shaft, float angle, VECTOR prevVec);

	//2点の距離(3次元)
	float LengthTwoPoint3D(VECTOR pos1, VECTOR pos2);

	//2つのベクトルのなす角
	float AngleTwoVector(VECTOR vec1, VECTOR vec2);

	//法線ベクトル
	VECTOR Normalize(VECTOR ver0, VECTOR ver1, VECTOR ver2);

	//カプセルと三角形それぞれの最近傍点
	void ClosestPointCapsuleAndTriangle(VECTOR capPos1, VECTOR capPos2, VECTOR ver0, VECTOR ver1, VECTOR ver2, VECTOR& capClosest, VECTOR& triangleClosest);

	/// <summary>
	/// 正射影ベクトル
	/// </summary>
	/// <param name="OA">原点から垂線を降ろされる側のベクトル</param>
	/// <param name="OB">原点から垂線を降ろす側のベクトル</param>
	/// <returns></returns>
	VECTOR OrthogonalProjectionVector(VECTOR OA, VECTOR OB);

	//バリセントリック座標計算
	void Barycentric(VECTOR ver0, VECTOR ver1, VECTOR ver2, VECTOR projectionPoint, float& u, float& v, float& w);

	//同じベクトルか判定
	bool SameVector(VECTOR vector1, VECTOR vector2);

	//三角形の中点
	VECTOR TriangleCenter(VECTOR vertex0, VECTOR vertex1, VECTOR vertex2);

	//回転行列のスケーリング成分を除去
	MATRIX NormalizeRotateMatrix(MATRIX matrix);

private:
	//線分上の最近傍点
	VECTOR ClosestPointOnSegment(VECTOR pos1, VECTOR pos2, const VECTOR point);

	//点と三角形の最近傍点
	VECTOR ClosestPointOnTriangle(VECTOR ver0, VECTOR ver1, VECTOR ver2, VECTOR point);
};