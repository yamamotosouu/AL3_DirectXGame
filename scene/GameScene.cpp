#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "MathUtilityForText.h"
#include "time.h"

//コントラクタ
GameScene::GameScene() {}
//デストラクタ
GameScene::~GameScene() { 

	delete spriteBG_;//BG
	delete modelStage_;//ステージ
	delete modelPlayer_;//プレイヤー
	delete modelBeam_;//ビーム
	delete modelEnemy_;//敵

}
//初期化
void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	//BG(2Dスプライト)
	textureHandleBG_ = TextureManager::Load("bg.jpg");
	spriteBG_ = Sprite::Create(textureHandleBG_, {0, 0});
	//ビュープロジェクションの初期化
	viewProjection_.translation_.y = 1;
	viewProjection_.translation_.z = -6;
	viewProjection_.Initialize();

	//ステージ
	textureHandleStage_ = TextureManager::Load("stage.jpg");
	modelStage_ = Model::Create();
	worldTransformStage_.Initialize();
	//ステージの位置を変更
	worldTransformStage_.translation_ = {0, -1.5f, 0};
	worldTransformStage_.scale_ = {4.5f, 1, 40};
	//変換行列を更新
	worldTransformStage_.matWorld_ = MakeAffineMatrix(
		worldTransformStage_.scale_,
	worldTransformStage_.rotation_,
	worldTransformStage_.translation_);
	//変換行列を定数バッファに転送
	worldTransformStage_.TransferMatrix();
	// ビーム
	textureHandleBeam_ = TextureManager::Load("beam.png");
	modelBeam_ = Model::Create();
	worldTransformBeam_.scale_ = {0.25f, 0.25f, 0.25f};
	worldTransformBeam_.Initialize();
	//プレイヤー
	textureHandlePlayer_ = TextureManager::Load("player.png");
	modelPlayer_ = Model::Create();
	worldTransformPlayer_.scale_ = {0.5f,0.5f,0.5f};
	worldTransformPlayer_.Initialize();
	// 敵
	textureHandleEnemy_ = TextureManager::Load("enemy.png");
	modelEnemy_ = Model::Create();
	worldTransformEnemy_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformEnemy_.Initialize();
	srand((unsigned int)time(NULL));//乱数の初期化
	//デバッグテキスト
	debugText_ = DebugText::GetInstance();
	debugText_->Initialize();
}
//更新
void GameScene::Update() {

	//各シーンの更新処理を呼び出す
	switch (sceneMode_)
	{
	case 0:
		GamePlayUpdate();//ゲームプレイ更新
		break;
	}

}

//描画
void GameScene::Draw() {


	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	//背景
	switch (sceneMode_) {
	case 0:
		GamePlayDraw2DBack(); // ゲームプレイ2D背景表示
		break;
	}
	/// </summary>

	// スプライト描画後処理


	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	
		switch (sceneMode_) {
	case 0:
		GamePlayDraw3D(); // ゲームプレイ3D表示
		break;
	}
	
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	
	switch (sceneMode_) {
	case 0:
		GamePlayDraw2DNear(); // ゲームプレイ2D近景表示
		break;
	}
	debugText_->DrawAll();
	
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
//プレイヤー
void GameScene::PlayerUpdate() 
{
	// 移動
	// 右に移動
	if (input_->PushKey(DIK_RIGHT)) {
		worldTransformPlayer_.translation_.x += 0.1f;
	}
	// 左に移動
	if (input_->PushKey(DIK_LEFT)) {
		worldTransformPlayer_.translation_.x -= 0.1f;
	}
	// 変換行列を更新
	worldTransformPlayer_.matWorld_ = MakeAffineMatrix(
	    worldTransformPlayer_.scale_, worldTransformPlayer_.rotation_,
	    worldTransformPlayer_.translation_);
	// 変換行列を定数バッファに転送
	worldTransformPlayer_.TransferMatrix();
}
//ビーム
void GameScene::BeamUpdate()
{
	//ビーム更新
	BeamMove();
	//ビーム発生
	BeamBorn();

	// 変換行列を更新
	worldTransformBeam_.matWorld_ = MakeAffineMatrix(
	    worldTransformBeam_.scale_, worldTransformBeam_.rotation_,
	    worldTransformBeam_.translation_);
	// 変換行列を定数バッファに転送
	worldTransformBeam_.TransferMatrix();

}
void GameScene::BeamMove()
{ 
	if (beamFlag_ == 1) {
		worldTransformBeam_.translation_.z += 1.0f;

		if (worldTransformBeam_.translation_.z >= 40) {
			beamFlag_ = 0;
		}
	}
	//回転
	worldTransformBeam_.rotation_.x += 0.1f;
	
}
void GameScene::BeamBorn() {

	if (beamFlag_ == 0) {
		if (input_->PushKey(DIK_SPACE)) {
			beamFlag_ = 1;
			worldTransformBeam_.translation_.x = worldTransformPlayer_.translation_.x;
			worldTransformBeam_.translation_.z = worldTransformPlayer_.translation_.z;
		}
	}
}
//敵
void GameScene::EnemyUpdate() 
{
	// 移動
	EnemyMove();
	//敵発生
	EnemyBorn();
	// 変換行列を更新
	worldTransformEnemy_.matWorld_ = MakeAffineMatrix(
	    worldTransformEnemy_.scale_, worldTransformEnemy_.rotation_,
	    worldTransformEnemy_.translation_);
	// 変換行列を定数バッファに転送
	worldTransformEnemy_.TransferMatrix();
}
void GameScene::EnemyMove()
{
	if (enemyFlag_ == 1) {
		worldTransformEnemy_.translation_.z -= 0.5f;

		if (worldTransformEnemy_.translation_.z < -5) {
			enemyFlag_ = 0;
		}
	}
	// 回転
	worldTransformEnemy_.rotation_.x -= 0.1f;
}
void GameScene::EnemyBorn() 
{ 
	if (enemyFlag_ == 0) {
		enemyFlag_ = 1;
		worldTransformEnemy_.translation_.z = 40;
		int x = rand() % 80;          // 80は4の10倍の2倍
		float x2 = (float)x / 10 - 4; // 10で割り、4を引く
		worldTransformEnemy_.translation_.x = x2;
	}
}
//衝突判定
void GameScene::Collision() {
	//衝突判定（プレイヤーと敵）
	CollisionPlayerEnemy();
	//衝突判定（ビームと敵）
	CollisionBeamEnemy();
}
void GameScene::CollisionPlayerEnemy() {
	//敵が存在すれば
	if (enemyFlag_ == 1) {
	    //差を求める
		float dx = abs(worldTransformPlayer_.translation_.x - worldTransformEnemy_.translation_.x);
		float dz = abs(worldTransformPlayer_.translation_.z - worldTransformEnemy_.translation_.z);
		//衝突したら
		if (dx < 1 && dz < 1) {
		//存在しない
			enemyFlag_ = 0;
			playerLife_ = playerLife_ - 1;
		}
	}
}
void GameScene::CollisionBeamEnemy() {
	// 敵とビームが存在すれば
	if (enemyFlag_ == 1&&beamFlag_==1) {
		// 差を求める
		float dx2 = abs(worldTransformBeam_.translation_.x - worldTransformEnemy_.translation_.x);
		float dz2 = abs(worldTransformBeam_.translation_.z - worldTransformEnemy_.translation_.z);
		// 衝突したら
		if (dx2 < 1 && dz2 < 1) {
			// 存在しない
			enemyFlag_ = 0;
			beamFlag_ = 0;
			gameScore_ = gameScore_ + 1;
		}
	}
}
//ゲームプレイ更新
void GameScene::GamePlayUpdate() {

	// プレイヤー更新
	PlayerUpdate();
	// ビーム更新
	BeamUpdate();
	// 敵更新
	EnemyUpdate();
	// 衝突判定
	Collision();

}
//ゲームプレイ表示3D
void GameScene::GamePlayDraw3D() {

	// ステージ
	modelStage_->Draw(worldTransformStage_, viewProjection_, textureHandleStage_);
	// プレイヤー
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);
	// ビーム
	if (beamFlag_ == 1) {
		modelPlayer_->Draw(worldTransformBeam_, viewProjection_, textureHandleBeam_);
	}
	// 敵
	if (enemyFlag_ == 1) {
		modelEnemy_->Draw(worldTransformEnemy_, viewProjection_, textureHandleEnemy_);
	}
}
// ゲームプレイ表示2D背景
void GameScene::GamePlayDraw2DBack() {
	// 背景
	spriteBG_->Draw();
}
// ゲームプレイ表示2D近景
void GameScene::GamePlayDraw2DNear() {

	// ゲームスコア
	char str[100];
	sprintf_s(str, "SCORE %d", gameScore_);
	debugText_->Print(str, 200, 10, 2);
	// プレイヤーライフ
	sprintf_s(str, "LIFE %d", playerLife_);
	debugText_->Print(str, 900, 10, 2);
	//テスト
	sprintf_s(str, "SCENE %d", sceneMode_);
	debugText_->Print(str, 500, 10, 2);

}

