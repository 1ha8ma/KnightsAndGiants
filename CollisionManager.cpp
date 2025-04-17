#include"DxLib.h"
#include"CollisionManager.h"

//初期化
CollisionManager* CollisionManager::collisionManager = nullptr;
std::vector<CollisionData*> CollisionManager::collisionDataList;

/// <summary>
/// コンストラクタ
/// </summary>
CollisionManager::CollisionManager()
{

}

/// <summary>
/// デストラクタ
/// </summary>
CollisionManager::~CollisionManager()
{
	collisionDataList.clear();
}

/// <summary>
/// 初期化
/// </summary>
void CollisionManager::Initialize()
{

}

/// <summary>
/// インスタンスゲット
/// </summary>
/// <returns>インスタンス</returns>
CollisionManager* CollisionManager::GetInstance()
{
	if (collisionManager == nullptr)
	{
		collisionManager = new CollisionManager();
	}

	return collisionManager;
}

/// <summary>
/// 当たり判定情報追加
/// </summary>
/// <param name="data">データ</param>
void CollisionManager::AddCollisionData(CollisionData* data)
{
	collisionDataList.push_back(data);
}

/// <summary>
/// 当たり判定情報削除
/// </summary>
/// <param name="data">データ</param>
void CollisionManager::RemoveCollisionData(CollisionData* data)
{
	int num = 0;
	for (int i = 0; i < collisionDataList.size(); i++)
	{
		if (collisionDataList[i] == data)
		{
			break;
		}
		else
		{
			num++;
		}
	}

	//削除
	collisionDataList.erase(collisionDataList.begin() + num);
}

/// <summary>
/// 全ての当たり判定処理
/// </summary>
void CollisionManager::Update()
{
	for (int i = 0; i < collisionDataList.size(); i++)
	{
		for (int j = 0; j < collisionDataList.size(); j++)
		{
			//同じものだったらスキップ
			if (i == j)
			{
				continue;
			}

			//どのタイプの当たり判定を行うか調べる
			CollisionData* data1 = collisionDataList[i];
			CollisionData* data2 = collisionDataList[j];

			//当たり判定を行って欲しいか確認
			if (data1->isCollisionActive && data2->isCollisionActive)
			{
				//カプセルどうし
				if (
					/*プレイヤーとステージのオブジェクト*/
					data1->tag == ObjectTag::PlayerWholeBody && data2->tag == ObjectTag::StageObject ||
					data1->tag == ObjectTag::PlayerFoot && data2->tag == ObjectTag::StageObject ||
					/*プレイヤーと敵*/
					data1->tag == ObjectTag::PlayerWholeBody && data2->tag == ObjectTag::Enemy ||
					data1->tag == ObjectTag::PlayerFoot && data2->tag == ObjectTag::Enemy ||
					/*敵の攻撃とプレイヤー*/
					data1->tag == ObjectTag::EnemyAttack && data2->tag == ObjectTag::PlayerWholeBody ||
					data1->tag == ObjectTag::EnemyAttack && data2->tag == ObjectTag::PlayerFoot ||
					/*プレイヤーの攻撃と敵*/
					data1->tag == ObjectTag::Attack_P && data2->tag == ObjectTag::Enemy ||
					data1->tag == ObjectTag::Attack_P && data2->tag == ObjectTag::WeakPoint
					)
				{
					bool hit = CapsuleWithCapsule(data1->startPosition, data1->endPosition, data1->radius, data2->startPosition, data2->endPosition, data2->radius);

					if (hit)
					{
						data1->HitProcess(data2);
						data2->HitProcess(data1);
					}
				}

				//カプセルとメッシュ
				if (
					/*敵の攻撃とステージオブジェクト*/
					data1->tag == ObjectTag::EnemyAttack && data2->tag == ObjectTag::StageObject
					)
				{
					//カプセルでの当たり判定をしてからメッシュの判定をするか判断
					bool isHitJudge = CapsuleWithCapsule(data1->startPosition, data1->endPosition, data1->radius, data2->startPosition, data2->endPosition, data2->radius);
					if (isHitJudge)
					{
						bool hit = CapsuleWithMesh(data1->startPosition, data1->endPosition, data1->radius, data2->meshData.polygonList);

						if (hit)
						{
							data1->HitProcess(data2);
							data2->HitProcess(data1);
						}
					}
				}
			}

		}
	}

	//プレイヤーと壁との当たり判定
	CollisionData wallData;		//壁の情報
	bool wallHit = false;		//壁に当たった
	//壁の情報を探して取る
	for (int i = 0; i < collisionDataList.size(); i++)
	{
		ObjectTag tag = collisionDataList[i]->tag;
		if (tag == ObjectTag::Stage)
		{
			wallData = *collisionDataList[i];
			break;
		}
	}
	//プレイヤーの情報を探して取り、押し戻すベクトルを計算
	for (int i = 0; i < collisionDataList.size(); i++)
	{
		CollisionData data = *collisionDataList[i];

		if (data.tag == ObjectTag::PlayerWholeBody)
		{
			VECTOR correction = VGet(0, 0, 0);
			if (data.position.x > wallData.stageRight)
			{
				float dif = data.position.x - wallData.stageRight;
				correction = VAdd(correction, VGet(-dif, 0, 0));
				wallHit = true;
			}
			if (data.position.x < wallData.stageLeft)
			{
				float dif = wallData.stageLeft - data.position.x;
				correction = VAdd(correction, VGet(dif, 0, 0));
				wallHit = true;
			}
			if (data.position.z > wallData.stageFront)
			{
				float dif = data.position.z - wallData.stageFront;
				correction = VAdd(correction, VGet(0, 0, -dif));
				wallHit = true;
			}
			if (data.position.z < wallData.stageBack)
			{
				float dif = wallData.stageBack - data.position.z;
				correction = VAdd(correction, VGet(0, 0, dif));
				wallHit = true;
			}

			//当たっていた場合
			if (wallHit)
			{
				data.WallHitProcess(correction);
			}
		}
	}
}

/// <summary>
/// カプセルどうしの当たり判定
/// </summary>
/// <param name="capsuleStart1">カプセル1始点</param>
/// <param name="capsuleEnd1">カプセル1終点</param>
/// <param name="capsuleRadius1">カプセル1半径</param>
/// <param name="capsuleStart2">カプセル2始点</param>
/// <param name="capsuleEnd2">カプセル2終点</param>
/// <param name="capsuleRadius2">カプセル2半径</param>
/// <returns>当たっているか</returns>
bool CollisionManager::CapsuleWithCapsule(VECTOR capsuleStart1, VECTOR capsuleEnd1, float capsuleRadius1, VECTOR capsuleStart2, VECTOR capsuleEnd2, float capsuleRadius2)
{
	bool hit = false;

	//カプセル間の最短距離
	float length = Segment_Segment_MinLength(capsuleStart1, capsuleEnd1, capsuleStart2, capsuleEnd2);

	//判定
	if (length < capsuleRadius1 + capsuleRadius2)
	{
		hit = true;
	}

	return hit;
}

/// <summary>
/// カプセルとメッシュのあたり判定
/// </summary>
/// <param name="capsuleStart">カプセル始点</param>
/// <param name="capsuleEnd">カプセル終点</param>
/// <param name="capsuleRadius">カプセル半径</param>
/// <param name="meshData">メッシュの情報</param>
/// <returns>当たったか</returns>
bool CollisionManager::CapsuleWithMesh(VECTOR capsuleStart, VECTOR capsuleEnd, float capsuleRadius, MV1_REF_POLYGONLIST meshData)
{
	bool result = false;

	for (int i = 0; i < meshData.PolygonNum; i++)
	{
		//三角形頂点
		VECTOR vertex0 = meshData.Vertexs[meshData.Polygons[i].VIndex[0]].Position;
		VECTOR vertex1 = meshData.Vertexs[meshData.Polygons[i].VIndex[1]].Position;
		VECTOR vertex2 = meshData.Vertexs[meshData.Polygons[i].VIndex[2]].Position;

		//カプセルと三角形の当たり判定
		result = HitCheck_Capsule_Triangle(capsuleStart, capsuleEnd, capsuleRadius, vertex0, vertex1, vertex2);

		if (result)
		{
			break;
		}
	}

	return result;
}

/// <summary>
/// 指定のオブジェクトの当たり判定情報を取る(当たった時だけでなくその後も最新の情報が欲しい時に使用)
/// </summary>
/// <param name="data">情報を取りたいオブジェクトのポインタ</param>
/// <returns>データ</returns>
CollisionData CollisionManager::GetCollisionData(CollisionData* data)
{
	int i;
	for (i = 0; i < collisionDataList.size(); i++)
	{
		if (collisionDataList[i] == data)
		{
			break;
		}
	}

	return *collisionDataList[i];
}