```
ProjectRoot/
├── Source/
│   ├── Main/                     // アプリケーションエントリポイントとメインクラス
│   │   └── WinMain.cpp
│   │   └── GameApp.h
│   │   └── GameApp.cpp
│   │
│   ├── Core/                     // DirectXコア、共通ユーティリティ
│   │   ├── D3D/
│   │   │   └── D3DDevice.h
│   │   │   └── Shader.h
│   │   ├── Renderer/
│   │   │   └── Renderer.h
│   │   │   └── PostProcessor.h   // ポストプロセッシングの追加
│   │   ├── Input/
│   │   │   └── InputManager.h
│   │   ├── Resource/
│   │   │   └── ResourceManager.h
│   │   └── Utility/
│   │       └── MathUtil.h
│   │
│   ├── Audio/                    // オーディオシステム（周波数解析含む）
│   │   └── AudioManager.h
│   │   └── AudioManager.cpp
│   │   └── AudioAnalyzer.h       // オーディオ周波数解析
│   │   └── AudioAnalyzer.cpp
│   │   └── MidiPlayer.h          // MIDI再生機能（任意）
│   │
│   ├── ActionGame/               // ★★ 2Dアクションパート（切り離し可能） ★★
│   │   ├── Scene/
│   │   │   └── ActionScene.h     // 2Dアクションのメインシーン
│   │   │   └── ActionScene.cpp
│   │   ├── Object/
│   │   │   └── ActionCharacter.h
│   │   │   └── FloatingNote.h    // 2Dアクション内のフローティングノート
│   │   ├── Effect/
│   │   │   └── ActionVisualEffect.h // アクションパート専用エフェクト
│   │   └── UI/
│   │       └── ActionHUD.h
│   │
│   ├── RhythmGame/               // ★★ 音ゲーパート ★★
│   │   ├── Scene/
│   │   │   └── RhythmGameScene.h // 音ゲーのメインシーン
│   │   │   └── RhythmGameScene.cpp
│   │   ├── Object/
│   │   │   └── Note.h            // 音ゲーのノートオブジェクト
│   │   │   └── Lane.h
│   │   ├── Judgment/
│   │   │   └── JudgmentManager.h
│   │   │   └── ScoreCalculator.h
│   │   ├── UI/
│   │   │   └── RhythmGameHUD.h
│   │   └── Effect/
│   │       └── HitEffect.h       // 音ゲーヒットエフェクト
│   │       └── RhythmVisualEffect.h // 音ゲーパート専用エフェクト
│   │
│   ├── ScoreAndMidi/             // ★★ MIDI解析と譜面生成 ★★
│   │   ├── MidiParser.h          // MIDI解析
│   │   ├── MidiParser.cpp
│   │   ├── ScoreGenerator.h      // 譜面生成
│   │   ├── ScoreGenerator.cpp
│   │   └── ScoreManager.h        // 譜面データ管理
│   │   └── ScoreManager.cpp
│   │   └── MidiDataTypes.h       // MIDI解析結果のデータ構造
│   │
│   └── Shared/                   // 共通定義など
│       └── Defines.h
│       └── Constants.h
│       └── GameState.h           // ゲーム全体のステート定義
│
├── Assets/                       // 各種リソースファイル
│   ├── Models/
│   ├── Textures/
│   ├── Shaders/
│   ├── Audio/
│   │   ├── BGM/
│   │   ├── SE/
│   │   └── MIDI/                 // MIDIファイル置き場
│   ├── Scores/                   // 生成された譜面データファイル
│   ├── Fonts/
│
├── Projects/                     // Visual Studioなどのプロジェクトファイル
│   └── MyRhythmActionGame.sln
│   └── MyRhythmActionGame.vcxproj
│
└── Docs/
```

エディタ付き
```
ProjectRoot/
├── Source/
│   ├── SharedCore/               // ★★ ゲームとエディタの共通基盤 ★★
│   │   ├── DataStructures/
│   │   │   └── CommonNoteData.h   // ノートの基本データ
│   │   │   └── ScoreData.h        // 譜面全体のデータ構造
│   │   ├── Serialization/
│   │   │   └── JsonSerializer.h
│   │   ├── Utility/
│   │   │   └── CommonMath.h
│   │   ├── AudioAnalysis/
│   │   │   └── AudioAnalyzer.h    // 周波数解析のコア部分
│   │   ├── Midi/
│   │   │   └── MidiParser.h       // MIDI解析のコア部分
│   │   │   └── MidiDataTypes.h
│   │   └── ScoreGeneration/
│   │       └── ScoreGenerator.h   // 自動譜面生成のコア部分
│   │
│   ├── GameSystem/               // ★★ ゲーム本体固有のロジック ★★
│   │   ├── Audio/
│   │   │   └── AudioManager.h     // ゲーム内オーディオ再生
│   │   ├── GameScenes/
│   │   │   └── ActionScene.h
│   │   │   └── RhythmGameScene.h
│   │   ├── GameObjects/
│   │   │   └── Note.h
│   │   │   └── FloatingNote.h
│   │   ├── Judgment/
│   │   ├── UI/
│   │   └── Effect/
│   │
│   ├── GameApp/                  // ゲーム本体のメインエントリポイント
│   │   └── WinMain.cpp
│   │   └── GameApp.h
│   │
│   ├── Renderer/                 // DirectX描画コア (ゲーム本体用)
│   │   └── D3DDevice.h
│   │   └── Renderer.h
│   │   └── PostProcessor.h
│   │
│   ├── InputManager/             // ゲーム本体の入力管理
│   │   └── InputManager.h
│   │
│   ├── ResourceManager/          // ゲーム本体のリソース管理
│   │   └── ResourceManager.h
│   │
│   ├── ScoreEditor/              // ★★ 譜面エディタツール ★★
│   │   ├── EditorApp/
│   │   │   └── EditorMain.cpp     // エディタのエントリポイント
│   │   │   └── EditorWindow.h     // GUIフレームワークの初期化など
│   │   ├── EditorViews/
│   │   │   └── TimelineView.h     // タイムライン表示ロジック
│   │   │   └── ScorePreviewView.h // 譜面プレビュー表示ロジック
│   │   │   └── PropertiesPanel.h
│   │   │   └── SpectrumView.h     // スペクトラム表示
│   │   ├── EditorLogic/
│   │   │   └── EditorScoreManager.h // 譜面編集ロジック
│   │   │   └── EditorMidiHandler.h  // MIDIロード、自動生成トリガー
│   │   │   └── EditorPreviewPlayer.h // エディタ内楽曲再生
│   │   ├── EditorUI/              // GUIフレームワークのラッパーなど
│   │   └── FileIO/
│
├── Assets/
│   ├── Models/
│   ├── Textures/
│   ├── Shaders/
│   ├── Audio/
│   │   ├── BGM/
│   │   ├── SE/
│   │   └── MIDI/                 // MIDIファイル
│   ├── Scores/                   // 生成・編集された譜面データ
│   ├── Fonts/
│
├── Projects/
│   ├── MyRhythmGame.sln
│   ├── GameProject.vcxproj       // ゲーム本体のプロジェクト
│   └── EditorProject.vcxproj     // エディタツールのプロジェクト
│
└── Docs/
    └── EditorManual.md           // エディタのユーザーマニュアル
```
