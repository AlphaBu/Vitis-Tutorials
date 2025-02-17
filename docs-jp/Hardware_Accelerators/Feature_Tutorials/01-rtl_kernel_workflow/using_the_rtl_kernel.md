<p align="right"><a href="../../../../README.md">English</a> | <a>日本語</a></p>
<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://japan.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.2 Vitis™ アプリケーション アクセラレーション開発フロー チュートリアル</h1><a href="https://github.com/Xilinx/Vitis-Tutorials/tree/2020.1">2020.1 Vitis アプリケーション アクセラレーション開発フロー チュートリアル</a></td>
 </tr>
</table>

# Vitis IDE プロジェクトでの RTL カーネルの使用

> **ヒント**: IP のパッケージ演習で作成した RTL カーネルを Vitis IDE で使用することもできます。

## ホスト コードの削除とインポート

Vivado ツールを終了すると、次のファイルが Vitis IDE の \[Project Explorer] ビューのハードウェア カーネル プロジェクト (`rtl_ke_t2_kernels`) に追加されます。

- `Vadd_A_B.xo`: コンパイル済みのカーネル オブジェクト ファイル。
- `host_example.cpp`: サンプルのホスト アプリケーション ファイル。

1. \[Project Explorer] ビューで次の図に示すように `rtl_ke_t2_kernels/src` フォルダーを展開します。

![\[Project Explorer] ビュー](./images/192_vitis_project_explorer.PNG)

> **注記:** `Vadd_A_B.xo` に稲妻アイコンが付いています。Vitis IDE では、これはハードウェア関数であることを示しています。IDE で RTL カーネルが認識され、アクセラレーションされた関数としてマークされます。

2. `host_example.cpp` を選択して削除します。

   この段階で、このチュートリアル用に提供されているホスト アプリケーションをインポートします。

3. \[Project Explorer] ビューで、プロセッサ プロジェクト (`rtl_ke_t2`) を右クリックし、**\[Import Sources]** をクリックします。

4. \[From directory] フィールドの **\[Browse]** をクリックして `01=rtl_kernel_workflow/reference-files/src` の `host` フォルダーのを指定し、**\[OK]** をクリックします。

5. `host.cpp` を選択してホスト アプリケーション コードをプロジェクトに追加します。

![ホスト コードのインポート](./images/import-host-code.png)

5. `Into folder` フィールドに `rtl_ke_t2/src` が表示されていることを確認し、**\[Finish]** をクリックします。

   `host.cpp` ファイルがホスト プロジェクトに追加されます。

6. \[Project Explorer] ビューで、右クリックして `Open` を選択 (または `host.cpp` をダブルクリック) して、\[Code Editor] ウィンドウで開きます。

## ホスト コードの構造

\[Code Editor] ウィンドウでホスト アプリケーションを確認します。ホスト コードの構造は、次の 3 つのセクションに分けられます。

1. OpenCL ランタイム環境の設定
2. カーネルの実行
3. FPGA デバイスのポストプロセスとリリース

ホスト アプリケーションと FPGA 間のデータ転送を可能にする重要な OpenCL API 呼び出しの一部を次に示します。

- カーネルへのハンドルを作成します (239 行目)。

  ```C
    clCreateKernel(program, "Vadd_A_B", &err);
  ```

- ホストと FPGA 間のデータ転送用にバッファーを作成します (256 行目)。

  ```C
  clCreateBuffer(context, CL_MEM_READ_WRITE,sizeof(int) * number_of_words, NULL, NULL);
  ```

- 値 (A および B) をバッファーに書き込み、そのバッファーを FPGA に転送します (266 および 274 行目)。

  ```C
  clEnqueueWriteBuffer(commands, dev_mem_ptr, CL_TRUE, 0,sizeof(int) * number_of_words, host_mem_ptr, 0, NULL, NULL);
  ```

- A と B がデバイスに転送された後、カーネルを実行します (299 行目)。

  ```C
  clEnqueueTask(command_queue, kernel, 0, NULL, NULL);
  ```

- カーネルが終了したら、ホスト アプリケーションが新しい B の値を含むバッファーを読み出します (312 行目)。

  ```C
  clEnqueueReadBuffer(command_queue, dev_mem_ptr, CL_TRUE, 0, sizeof(int)*number_of_words,host_mem_output_ptr, 0, NULL, &readevent );
  ```

ホスト アプリケーションの構造および要件の詳細は、『Vitis 統合ソフトウェア プラットフォームの資料』 (UG1416) のアプリケーション アクセラレーション開発フローの[アプリケーションの開発](https://japan.xilinx.com/cgi-bin/docs/rdoc?v=2020.2;t=vitis+doc;d=lhv1569273988420.html)を参照してください。

## ハードウェア関数の追加

ホスト アプリケーション コード (`host.cpp`) をホスト プロジェクトに追加し、RTL カーネル コード (`Vadd_A_B.xo`) をカーネル プロジェクトに追加したら、ハードウェア関数を指定してデバイス バイナリ または `.xclbin` ファイルを生成します。

1. HW カーネル プロジェクトの `rtl_ke_t2_kernels/rtl_ke_t2_kernels.prj` を右クリック 、**\[Open]** を使用してプロジェクトを開きます。

2. プロジェクト エディターの **\[Hardware Functions]** エリアで、![ハードウェア関数](./images/lightning_icon.PNG) をクリックしてプロジェクトにハードウェア関数を追加します。

3. `Vadd_A_B` 関数を選択します。

   ![ハードウェア関数を追加](./images/add-hw-function.png)

4. **\[OK]** をクリックします。

## プロジェクトのビルド

ホスト アプリケーション コード (`host.cpp`) および RTL カーネル コード (`Vadd_A_B.xo`) をプロジェクトに追加したら、プロジェクトをビルドして実行できます。

> **ヒント**: IP/XO のパッケージ演習で作成した RTL カーネルを Vitis IDE で使用できます。`.xo` カーネル ファイルを `rtl_ke_t2_kernels/src` フォルダーにインポートし、ハードウェア関数として指定します。

1. \[Hardware Kernel Project Settings] で **\[Active build configuration]** を **\[Emulation-HW]** に変更します。  
ハードウェア エミュレーションは、次のような場合に有益です。

   - FPGA に含まれるロジックの機能を検証。
   - アクセラレータおよびホスト アプリケーションの初期パフォーマンス見積もりを取得。

   > **ヒント:** ソフトウェア エミュレーションでは、RTL カーネル フローにそのカーネルの C/C++ ソフトウェア モデルが必要です。このチュートリアルにはそのようなモデルは含まれていないので、ソフトウェア エミュレーション ビルドを実行することはできません。

2. \[Assistant] ビューで、'rtl\_ke\_t2\_system\` 最上位のシステム プロジェクトを選択し、Build コマンド (![\[Build] コマンド](./images/icon_build.png)) をクリック してアクティブな \[Emulation HW] ビルド コンフィギュレーションをビルドします。

   Vitis アプリケーション プロジェクトのさまざまな要素 (プロセッサ コード (`host.cpp`)、RTL カーネル (.xo) をターゲット プラットフォームにリンクするハードウェア リンク プロジェクト、デザインをパッケージする最上位システム プロジェクト) がビルドされます。

   > **ヒント**: RTL カーネルは `.xo` ファイルとしてインポートされるため、カーネルをコンパイルする必要はありません。

3. \[Assistant] ビューで **\[Run]** ボタンをクリックし、**\[Run Configurations]** を選択します。

4. `System Project Debug` コンフィギュレーションを選択し、**\[New launch configuration]** コマンド (![コンフィギュレーションの開始](./images/icon-new-launch-config.png)) をクリックして、実行用の新しいコンフィギュレーションを作成します。

   `SystemDebugger_rtl_ke_t2_system` コンフィギュレーションが作成されます。

   ホスト プログラムは、`xclbin` ファイルとターゲット プラットフォームを入力引数として受け取ります。これは、\[Program Arguments] リストで指定する必要があります。 ![XCLBIN](./images/xclbin.PNG)

5. `Program Arguments` の横にある **\[Edit]** をクリックします。

**\[Automatically update arguments]** がオンになっている場合は、`xclbin` ファイルが自動的に検索され、含まれます。ただし、この場合は 2 つの引数を追加する必要があるため、このチェック ボックスをオフにします。

このチェック ボックスをオフにし、次の図に示すように、`Edit Program Arguments` ダイアログ ボックスの `Program Arguments` フィールドに **.../binary\_container\_1.xclbin xilinx\_u200\_xdma\_201830\_2** と入力します。

![プログラム引数を編集](./images/program-arguments.png)

5. **\[OK]** をクリックして引数を追加します。

6. `Run Configurations` ダイアログボックスで **\[Apply]** をクリックし、**\[Run]** をクリックしてコンフィギュレーションを実行し、結果を確認します。

Vitis IDE の \[Console] ウィンドウに「**INFO: Test Completed**」と表示されます。RTL カーネルを使用してアプリケーションをビルドして実行しました。

### (オプション) ターゲット プラットフォームでのハードウェアのビルドと実行

1. \[Vitis Application Project Settings] で **\[Active build configuration]** を **\[Hardware]** に変更します。  
システム コンフィギュレーションでは、カーネル コードが FPGA にインプリメントされるので、選択したプラットフォーム カードで実行されるバイナリが生成されます。

2. 使用可能なハードウェア プラットフォームがある場合は、ハードウェアをビルドして実行し、結果を検証してください。

# まとめ

このチュートリアルでは、**\[Package IP/Package\_XO]** フローを使用して RTL カーネルを作成し、**RTL Kernel ウィザード**も使用しました。RTL IP プロジェクトを Vitis ツールで必要とされるコンパイル済み XO ファイルにパッケージしました。そのあと、RTL カーネルをアプリケーション プロジェクトに追加し、ホスト コードと組み合わせて、ハードウェア エミュレーション コンフィギュレーションをビルドして実行しました。  Vitis IDE で、XO ファイルを使用してバイナリ コンテナーを作成し、`xclbin` ファイルをコンパイルしました。

</br><hr/>
<p align="center"><sup>Copyright&copy; 2020 Xilinx</sup></p>
<p align= center class="sphinxhide"><b><a href="../../../README.md">メイン ページに戻る</a> &mdash; <a href="../../README.md/">ハードウェア アクセラレータ チュートリアルの初めに戻る</a></b></p></br>
<p align="center"><sup>この資料は 2021 年 2 月 8 日時点の表記バージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。
日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。</sup></p>
