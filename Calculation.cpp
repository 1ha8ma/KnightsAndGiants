#include<math.h>
#include<algorithm>
#include"DxLib.h"
#include"Calculation.h"

/// <summary>
/// 線形補間
/// </summary>
/// <param name="targetPos">目標のポジション</param>
/// <param name="nowPos">現在のポジション</param>
/// <param name="speed">速度</param>
/// <param name="t">割合</param>
VECTOR Calculation::Lerp(VECTOR targetPos, VECTOR nowPos, float speed, float &t)
{
	VECTOR copypos = nowPos;
	copypos = VAdd(copypos, VScale((VSub(targetPos, copypos)), t));

	//近づける
	if (t < 1)
	{
		t += speed;
		if (t >= 1)
		{
			t = 1;
		}
	}

	return copypos;
}

/// <summary>
/// ロドリゲスの回転公式
/// </summary>
/// <param name="shaft">軸</param>
/// <param name="angle">回転角度</param>
/// <param name="prevVec">回転前のベクトル</param>
/// <returns>回転後のベクトル</returns>
VECTOR Calculation::RodriguesRotationFormula(VECTOR shaft, float angle, VECTOR prevVec)
{
	VECTOR unitVec = VNorm(shaft);//単位ベクトル
	VECTOR newVec;//回転後のベクトル

	VECTOR calc1 = VScale(prevVec, cos(angle));
	VECTOR calc2 = VScale(unitVec, VDot(prevVec, unitVec));
	calc2 = VScale(calc2, (1 - cos(angle)));
	VECTOR calc3 = VCross(unitVec, prevVec);
	calc3 = VScale(calc3, sin(angle));

	newVec = VAdd(calc1, calc2);
	newVec = VAdd(newVec, calc3);

	return newVec;
}

/// <summary>
/// 2点の距離(3次元)
/// </summary>
/// <param name="pos1">点1</param>
/// <param name="pos2">点2</param>
/// <returns>距離</returns>
float Calculation::LengthTwoPoint3D(VECTOR pos1, VECTOR pos2)
{
	float calc1 = pow(pos2.x - pos1.x, 2) + pow(pos2.y - pos1.y, 2) + pow(pos2.z - pos1.z, 2);
	float length = sqrtf(calc1);
	
	return length;
}

/// <summary>
/// 2つのベクトルのなす角
/// </summary>
/// <param name="vec1">ベクトル1</param>
/// <param name="vec2">ベクトル2</param>
/// <returns>角度</returns>
float Calculation::AngleTwoVector(VECTOR vec1, VECTOR vec2)
{
	float calc1 = VDot(vec1, vec2);
	float calc2 = sqrtf(pow(vec1.x, 2) + pow(vec1.y, 2) + pow(vec1.z, 2)) * sqrtf(pow(vec2.x, 2) + pow(vec2.y, 2) + pow(vec2.z, 2));
	float cosTheta = calc1 / calc2;

	float angle = acos(cosTheta);

	float deg = angle * (180 / DX_PI_F);//確認用

	return angle;
}

/// <summary>
/// 正射影ベクトル
/// </summary>
/// <param name="OA">原点から垂線を降ろされる側のベクトル</param>
/// <param name="OB">原点から垂線を降ろす側のベクトル</param>
/// <returns>降ろした点</returns>
VECTOR Calculation::OrthogonalProjectionVector(VECTOR OA, VECTOR OB)
{
	float calc1 = VDot(OA, OB);
	float calc2 = pow(OA.x, 2) + pow(OA.y, 2) + pow(OA.z, 2);
	float calc3 = calc1 / calc2;
	VECTOR OH = VScale(OA, calc3);

	return OH;
}

/// <summary>
/// 法線ベクトル
/// </summary>
/// <param name="ver0">頂点1</param>
/// <param name="ver1">頂点2</param>
/// <param name="ver2">頂点3</param>
/// <returns>法線ベクトル</returns>
VECTOR Calculation::Normalize(VECTOR ver0, VECTOR ver1, VECTOR ver2)
{
	VECTOR vec01 = VSub(ver1, ver0);		//0→1
	VECTOR vec02 = VSub(ver2, ver0);		//0→2
	VECTOR normVec = VCross(vec01, vec02);	//法線ベクトル
	normVec = VNorm(normVec);

	return normVec;
}

/// <summary>
/// カプセルと三角形のそれぞれの最近傍点
/// </summary>
/// <param name="capPos1">カプセルポジション1</param>
/// <param name="capPos2">カプセルポジション2</param>
/// <param name="ver0">頂点1</param>
/// <param name="ver1">頂点2</param>
/// <param name="ver2">頂点3</param>
/// <param name="capClosest">カプセル最近傍点</param>
/// <param name="triangleClosest">三角形最近傍点</param>
void Calculation::ClosestPointCapsuleAndTriangle(VECTOR capPos1, VECTOR capPos2, VECTOR ver0, VECTOR ver1, VECTOR ver2, VECTOR& capClosest, VECTOR& triangleClosest)
{
	VECTOR closestA = ClosestPointOnTriangle(ver0, ver1, ver2, capPos1);
	VECTOR closestB = ClosestPointOnTriangle(ver0, ver1, ver2, capPos2);

	float distA = LengthTwoPoint3D(capPos1, closestA);
	float distB = LengthTwoPoint3D(capPos2, closestB);

	if (distA < distB)
	{
		capClosest = capPos1;
		triangleClosest = closestA;
	}
	else
	{
		capClosest = capPos2;
		triangleClosest = closestB;
	}
}

/// <summary>
/// 線分上の最近傍点
/// </summary>
/// <param name="pos1">ポジション1</param>
/// <param name="pos2">ポジション2</param>
/// <param name="point">点</param>
/// <returns>最近傍点</returns>
VECTOR Calculation::ClosestPointOnSegment(VECTOR pos1, VECTOR pos2, const VECTOR point)
{
	VECTOR vec12 = VSub(pos2, pos1);
	VECTOR vec1p = VSub(point, pos1);
	float dot = VDot(vec1p, vec12);
	float len = LengthTwoPoint3D(pos1, pos2);
	len = VDot(vec12, vec12);
	float t = dot / len;

	//線分内に納める
	if (t < 0.0f)
	{
		t = 0.0f;
	}
	if (t > 1.0f)
	{
		t = 1.0f;
	}

	VECTOR closestPoint = VAdd(pos1, VScale(vec12, t));

	return closestPoint;
}

/// <summary>
/// 点と三角形の最近傍点を求める
/// </summary>
/// <param name="ver1">頂点1</param>
/// <param name="ver2">頂点2</param>
/// <param name="ver3">頂点3</param>
/// <param name="point">点</param>
/// <returns>最近傍点</returns>
VECTOR Calculation::ClosestPointOnTriangle(VECTOR ver0, VECTOR ver1, VECTOR ver2, VECTOR point)
{
	//三角形の外側にある場合は最近傍点を求める
	VECTOR closest01 = ClosestPointOnSegment(ver0, ver1, point);
	VECTOR closest12 = ClosestPointOnSegment(ver1, ver2, point);
	VECTOR closest20 = ClosestPointOnSegment(ver2, ver0, point);

	//最近傍点とpointの最も距離の近いもの
	float distance01 = LengthTwoPoint3D(point, closest01);
	float distance12 = LengthTwoPoint3D(point, closest12);
	float distance20 = LengthTwoPoint3D(point, closest20);

	if (distance01 < distance12 && distance01 < distance20)
	{
		return closest01;
	}
	if (distance12 < distance20)
	{
		return closest12;
	}
	else
	{
		return closest20;
	}
}

/// <summary>
/// バリセントリック座標計算
/// </summary>
/// <param name="ver0">頂点1</param>
/// <param name="ver1">頂点2</param>
/// <param name="ver2">頂点3</param>
/// <param name="projectionPoint">三角形に投影した点</param>
/// <param name="u">中心座標</param>
/// <param name="v">中心座標</param>
/// <param name="w">中心座標</param>
void Calculation::Barycentric(VECTOR ver0, VECTOR ver1, VECTOR ver2, VECTOR projectionPoint, float& u, float& v, float& w)
{
	VECTOR v0 = VSub(ver1, ver0);
	VECTOR v1 = VSub(ver2, ver0);
	VECTOR v2 = VSub(projectionPoint, ver0);

	float dot00 = VDot(v0, v0);
	float dot01 = VDot(v0, v1);
	float dot11 = VDot(v1, v1);
	float dot20 = VDot(v2, v0);
	float dot21 = VDot(v2, v1);
	float denom = (dot00 * dot11) - (dot01 * dot01);

	v = ((dot11 * dot20) - (dot01 * dot21)) / denom;
	w = ((dot00 * dot21) - (dot01 * dot20)) / denom;
	u = 1.0f - v - w;
}

/// <summary>
/// 同じベクトルか判定
/// </summary>
/// <param name="vector1">ベクトル1</param>
/// <param name="vector2">ベクトル2</param>
/// <returns>結果</returns>
bool Calculation::SameVector(VECTOR vector1, VECTOR vector2)
{
	bool same = false;

	if (vector1.x == vector2.x &&
		vector1.y == vector2.y &&
		vector1.z == vector2.z)
	{
		same = true;
	}

	return same;
}

/// <summary>
/// 三角形の中点
/// </summary>
/// <param name="vertex0">頂点0</param>
/// <param name="vertex1">頂点1</param>
/// <param name="vertex2">頂点2</param>
/// <returns>中点ポジション</returns>
VECTOR Calculation::TriangleCenter(VECTOR vertex0, VECTOR vertex1, VECTOR vertex2)
{
	VECTOR centerPos;
	centerPos = VAdd(VAdd(vertex0, vertex1), vertex2);
	centerPos.x = centerPos.x / 3;
	centerPos.y = centerPos.y / 3;
	centerPos.z = centerPos.z / 3;

	return centerPos;
}

/// <summary>
/// 回転行列のスケーリング成分を除去
/// </summary>
/// <param name="matrix">行列</param>
/// <returns>除去後の回転行列</returns>
MATRIX Calculation::NormalizeRotateMatrix(MATRIX matrix)
{
	VECTOR xAxis = VNorm(VGet(matrix.m[0][0], matrix.m[1][0], matrix.m[2][0]));	//X軸正規化
	VECTOR yAxis = VNorm(VGet(matrix.m[0][1], matrix.m[1][1], matrix.m[2][1]));	//Y軸正規化
	VECTOR zAxis = VNorm(VGet(matrix.m[0][2], matrix.m[1][2], matrix.m[2][2]));	//Z軸正規化

	MATRIX normalizeMatrix =
	{
		xAxis.x, yAxis.x, zAxis.x, 0.0f,
		xAxis.y, yAxis.y, zAxis.y, 0.0f,
		xAxis.z, yAxis.z, zAxis.z, 0.0f,
		0.0f,    0.0f,    0.0f,    1.0f
	};

	return normalizeMatrix;
}