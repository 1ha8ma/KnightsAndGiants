#include<math.h>
#include<algorithm>
#include"DxLib.h"
#include"Calculation.h"

/// <summary>
/// ���`���
/// </summary>
/// <param name="targetPos">�ڕW�̃|�W�V����</param>
/// <param name="nowPos">���݂̃|�W�V����</param>
/// <param name="speed">���x</param>
/// <param name="t">����</param>
VECTOR Calculation::Lerp(VECTOR targetPos, VECTOR nowPos, float speed, float &t)
{
	VECTOR copypos = nowPos;
	copypos = VAdd(copypos, VScale((VSub(targetPos, copypos)), t));

	//�߂Â���
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
/// ���h���Q�X�̉�]����
/// </summary>
/// <param name="shaft">��</param>
/// <param name="angle">��]�p�x</param>
/// <param name="prevVec">��]�O�̃x�N�g��</param>
/// <returns>��]��̃x�N�g��</returns>
VECTOR Calculation::RodriguesRotationFormula(VECTOR shaft, float angle, VECTOR prevVec)
{
	VECTOR unitVec = VNorm(shaft);//�P�ʃx�N�g��
	VECTOR newVec;//��]��̃x�N�g��

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
/// 2�_�̋���(3����)
/// </summary>
/// <param name="pos1">�_1</param>
/// <param name="pos2">�_2</param>
/// <returns>����</returns>
float Calculation::LengthTwoPoint3D(VECTOR pos1, VECTOR pos2)
{
	float calc1 = pow(pos2.x - pos1.x, 2) + pow(pos2.y - pos1.y, 2) + pow(pos2.z - pos1.z, 2);
	float length = sqrtf(calc1);
	
	return length;
}

/// <summary>
/// 2�̃x�N�g���̂Ȃ��p
/// </summary>
/// <param name="vec1">�x�N�g��1</param>
/// <param name="vec2">�x�N�g��2</param>
/// <returns>�p�x</returns>
float Calculation::AngleTwoVector(VECTOR vec1, VECTOR vec2)
{
	float calc1 = VDot(vec1, vec2);
	float calc2 = sqrtf(pow(vec1.x, 2) + pow(vec1.y, 2) + pow(vec1.z, 2)) * sqrtf(pow(vec2.x, 2) + pow(vec2.y, 2) + pow(vec2.z, 2));
	float cosTheta = calc1 / calc2;

	float angle = acos(cosTheta);

	float deg = angle * (180 / DX_PI_F);//�m�F�p

	return angle;
}

/// <summary>
/// ���ˉe�x�N�g��
/// </summary>
/// <param name="OA">���_���琂�����~�낳��鑤�̃x�N�g��</param>
/// <param name="OB">���_���琂�����~�낷���̃x�N�g��</param>
/// <returns>�~�낵���_</returns>
VECTOR Calculation::OrthogonalProjectionVector(VECTOR OA, VECTOR OB)
{
	float calc1 = VDot(OA, OB);
	float calc2 = pow(OA.x, 2) + pow(OA.y, 2) + pow(OA.z, 2);
	float calc3 = calc1 / calc2;
	VECTOR OH = VScale(OA, calc3);

	return OH;
}

/// <summary>
/// �@���x�N�g��
/// </summary>
/// <param name="ver0">���_1</param>
/// <param name="ver1">���_2</param>
/// <param name="ver2">���_3</param>
/// <returns>�@���x�N�g��</returns>
VECTOR Calculation::Normalize(VECTOR ver0, VECTOR ver1, VECTOR ver2)
{
	VECTOR vec01 = VSub(ver1, ver0);		//0��1
	VECTOR vec02 = VSub(ver2, ver0);		//0��2
	VECTOR normVec = VCross(vec01, vec02);	//�@���x�N�g��
	normVec = VNorm(normVec);

	return normVec;
}

/// <summary>
/// �J�v�Z���ƎO�p�`�̂��ꂼ��̍ŋߖT�_
/// </summary>
/// <param name="capPos1">�J�v�Z���|�W�V����1</param>
/// <param name="capPos2">�J�v�Z���|�W�V����2</param>
/// <param name="ver0">���_1</param>
/// <param name="ver1">���_2</param>
/// <param name="ver2">���_3</param>
/// <param name="capClosest">�J�v�Z���ŋߖT�_</param>
/// <param name="triangleClosest">�O�p�`�ŋߖT�_</param>
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
/// ������̍ŋߖT�_
/// </summary>
/// <param name="pos1">�|�W�V����1</param>
/// <param name="pos2">�|�W�V����2</param>
/// <param name="point">�_</param>
/// <returns>�ŋߖT�_</returns>
VECTOR Calculation::ClosestPointOnSegment(VECTOR pos1, VECTOR pos2, const VECTOR point)
{
	VECTOR vec12 = VSub(pos2, pos1);
	VECTOR vec1p = VSub(point, pos1);
	float dot = VDot(vec1p, vec12);
	float len = LengthTwoPoint3D(pos1, pos2);
	len = VDot(vec12, vec12);
	float t = dot / len;

	//�������ɔ[�߂�
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
/// �_�ƎO�p�`�̍ŋߖT�_�����߂�
/// </summary>
/// <param name="ver1">���_1</param>
/// <param name="ver2">���_2</param>
/// <param name="ver3">���_3</param>
/// <param name="point">�_</param>
/// <returns>�ŋߖT�_</returns>
VECTOR Calculation::ClosestPointOnTriangle(VECTOR ver0, VECTOR ver1, VECTOR ver2, VECTOR point)
{
	//�O�p�`�̊O���ɂ���ꍇ�͍ŋߖT�_�����߂�
	VECTOR closest01 = ClosestPointOnSegment(ver0, ver1, point);
	VECTOR closest12 = ClosestPointOnSegment(ver1, ver2, point);
	VECTOR closest20 = ClosestPointOnSegment(ver2, ver0, point);

	//�ŋߖT�_��point�̍ł������̋߂�����
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
/// �o���Z���g���b�N���W�v�Z
/// </summary>
/// <param name="ver0">���_1</param>
/// <param name="ver1">���_2</param>
/// <param name="ver2">���_3</param>
/// <param name="projectionPoint">�O�p�`�ɓ��e�����_</param>
/// <param name="u">���S���W</param>
/// <param name="v">���S���W</param>
/// <param name="w">���S���W</param>
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
/// �����x�N�g��������
/// </summary>
/// <param name="vector1">�x�N�g��1</param>
/// <param name="vector2">�x�N�g��2</param>
/// <returns>����</returns>
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
/// �O�p�`�̒��_
/// </summary>
/// <param name="vertex0">���_0</param>
/// <param name="vertex1">���_1</param>
/// <param name="vertex2">���_2</param>
/// <returns>���_�|�W�V����</returns>
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
/// ��]�s��̃X�P�[�����O����������
/// </summary>
/// <param name="matrix">�s��</param>
/// <returns>������̉�]�s��</returns>
MATRIX Calculation::NormalizeRotateMatrix(MATRIX matrix)
{
	VECTOR xAxis = VNorm(VGet(matrix.m[0][0], matrix.m[1][0], matrix.m[2][0]));	//X�����K��
	VECTOR yAxis = VNorm(VGet(matrix.m[0][1], matrix.m[1][1], matrix.m[2][1]));	//Y�����K��
	VECTOR zAxis = VNorm(VGet(matrix.m[0][2], matrix.m[1][2], matrix.m[2][2]));	//Z�����K��

	MATRIX normalizeMatrix =
	{
		xAxis.x, yAxis.x, zAxis.x, 0.0f,
		xAxis.y, yAxis.y, zAxis.y, 0.0f,
		xAxis.z, yAxis.z, zAxis.z, 0.0f,
		0.0f,    0.0f,    0.0f,    1.0f
	};

	return normalizeMatrix;
}