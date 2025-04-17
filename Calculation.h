#pragma once

class Calculation
{
public:
	Calculation() {};
	~Calculation() {};

	//���`���
	VECTOR Lerp(VECTOR targetPos, VECTOR nowPos, float speed, float& t);

	//���h���Q�X�̉�]����
	VECTOR RodriguesRotationFormula(VECTOR shaft, float angle, VECTOR prevVec);

	//2�_�̋���(3����)
	float LengthTwoPoint3D(VECTOR pos1, VECTOR pos2);

	//2�̃x�N�g���̂Ȃ��p
	float AngleTwoVector(VECTOR vec1, VECTOR vec2);

	//�@���x�N�g��
	VECTOR Normalize(VECTOR ver0, VECTOR ver1, VECTOR ver2);

	//�J�v�Z���ƎO�p�`���ꂼ��̍ŋߖT�_
	void ClosestPointCapsuleAndTriangle(VECTOR capPos1, VECTOR capPos2, VECTOR ver0, VECTOR ver1, VECTOR ver2, VECTOR& capClosest, VECTOR& triangleClosest);

	/// <summary>
	/// ���ˉe�x�N�g��
	/// </summary>
	/// <param name="OA">���_���琂�����~�낳��鑤�̃x�N�g��</param>
	/// <param name="OB">���_���琂�����~�낷���̃x�N�g��</param>
	/// <returns></returns>
	VECTOR OrthogonalProjectionVector(VECTOR OA, VECTOR OB);

	//�o���Z���g���b�N���W�v�Z
	void Barycentric(VECTOR ver0, VECTOR ver1, VECTOR ver2, VECTOR projectionPoint, float& u, float& v, float& w);

	//�����x�N�g��������
	bool SameVector(VECTOR vector1, VECTOR vector2);

	//�O�p�`�̒��_
	VECTOR TriangleCenter(VECTOR vertex0, VECTOR vertex1, VECTOR vertex2);

	//��]�s��̃X�P�[�����O����������
	MATRIX NormalizeRotateMatrix(MATRIX matrix);

private:
	//������̍ŋߖT�_
	VECTOR ClosestPointOnSegment(VECTOR pos1, VECTOR pos2, const VECTOR point);

	//�_�ƎO�p�`�̍ŋߖT�_
	VECTOR ClosestPointOnTriangle(VECTOR ver0, VECTOR ver1, VECTOR ver2, VECTOR point);
};