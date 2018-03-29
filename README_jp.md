# Caffe2とQt5によるAndroid AIアプリのデモ

Qt5とCaffe2で作るシンプルなAIアプリのデモです。

### 解説

有名なディープラーニングフレームワークであるCaffe2と、素晴らしいGUIツールキットであるQt、どちらもモバイル環境をサポートし、C++で書かれています。私のアイデアは単純にこういうことです。「もしこれらをミックスすることができたらandroid向けのディープラーニングアプリをc++だけで書けるんじゃない？」

### デモ
![result](https://github.com/yamsam/Qt5Caffe2AndroidDemo/blob/media/demo.gif)

### ビルド
1) Qt5(android_armv7) と Qt Creatorを準備します 
   - Qt 5.10.1とqt creatror 4.4.1で、windows 10とubuntu 16.04上でテストしました
2) Caffe2アンドロイドライブラリを作る
   - 自分でCaffe2をビルドするか、私のビルドしたバイナリを使ってください (https://drive.google.com/drive/folders/1dqYYQkEszpO6efFXN4blUEaRR8BgDTYt?usp=sharing)  
   - ライブラリをプロジェクトディレクトリ下のcaffe2libに置いてください。
3) Qt5 android_armv7キットでコンパイルし、 アンドロイドにデプロイします

### 使い方
画面をタッチすると、プログラムはディスプレイ画像の中央部分を切り取ってニューラルネットにわたし、画像の分類を実行します。実行結果は画面上部に表示されます。

### 特徴
* ディープラーニングフレームワークを使って画像を分類します
* QtQuick CameraとQAbstractVideoFilterを使ってディスプレイ画像をキャプチャーします
* 軽量なニューラルネットであるSqueeze Netを使用しています。これは1000クラスのImageNetで学習されています

### 関連プロジェクト
以下のプロジェクトをベースに作成しました。
 * https://github.com/caffe2/AICamera
 * https://github.com/stephenquan/MyVideoFilterApp

### ライセンス
This project is licensed under the MIT License
