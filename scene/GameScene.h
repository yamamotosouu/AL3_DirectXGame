#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugText.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();
	//ビュープロジェクション(共通)
	ViewProjection viewProjection_;

	//ステージ
	uint32_t textureHandleStage_ = 0;
	Model* modelStage_ = nullptr;
	WorldTransform worldTransformStage_;

	//プレイヤー
	uint32_t textureHandlePlayer_ = 0;
	Model* modelPlayer_ = nullptr;
	WorldTransform worldTransformPlayer_;

	//ビーム
	uint32_t textureHandleBeam_ = 0;
	Model* modelBeam_ = nullptr;
	WorldTransform worldTransformBeam_;
	int beamFlag_ = 0;//ビーム存在フラグ(0:存在しない、1:存在する)
	
	// 敵
	uint32_t textureHandleEnemy_ = 0;
	Model* modelEnemy_ = nullptr;
	WorldTransform worldTransformEnemy_;
	int enemyFlag_ = 0;//敵存在フラグ(0:存在しない、1:存在する)


	//デバッグテキスト
	DebugText* debugText_ = nullptr;
	int gameScore_ = 0;//ゲームスコア
	int playerLife_ = 3;//プレイヤーライフ

	//シーン切り替え
	int sceneMode_ = 0;//シーンモード（0:ゲームプレイ　1:タイトル)

	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
//プレイヤー更新
private:
	void PlayerUpdate();
//ビーム更新
private:
	void BeamUpdate();
//ビーム移動
private:
	void BeamMove();
//ビーム発射
private:
	void BeamBorn();
//敵更新
private:
	void EnemyUpdate();
//敵移動
private:
	void EnemyMove();
//敵発生
private:
	void EnemyBorn();
//衝突判定
private:
	void Collision();
//衝突判定(プレイヤーと敵)
private:
	void CollisionPlayerEnemy();
// 衝突判定(ビームと敵)
private:
	void CollisionBeamEnemy();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	//BG(スプライト)
	uint32_t textureHandleBG_ = 0;
	Sprite* spriteBG_ = nullptr;
//ゲームプレイ更新
private:
	void GamePlayUpdate();
//ゲームプレイ3D表示
private:
	void GamePlayDraw3D();
//ゲームプレイ背景2D表示
private:
	void GamePlayDraw2DBack();
// ゲームプレイ近景2D表示
private:
	void GamePlayDraw2DNear();

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
