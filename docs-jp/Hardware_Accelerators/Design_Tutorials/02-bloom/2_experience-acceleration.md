<p align="right"><a href="../../../../README.md">English</a> | <a>日本語</a></p>
<table class="sphinxhide">
 <tr>
   <td align="center"><img src="https://japan.xilinx.com/content/dam/xilinx/imgs/press/media-kits/corporate/xilinx-logo.png" width="30%"/><h1>2020.2 Vitis™ アプリケーション アクセラレーション開発フロー チュートリアル</h1><a href="https://github.com/Xilinx/Vitis-Tutorials/tree/2020.1">2020.1 Vitis アプリケーション アクセラレーション開発フロー チュートリアル</a></td>
 </tr>
</table>

# アクセラレーション パフォーマンスの体験

この演習では、アプリケーションをまずソフトウェアのみのバージョンで実行し、その後コンパイル済み FPGA アクセラレータを使用して最適化されたアクセラレーション バージョンを実行し、アクセラレーションの可能性を体験します。

1. 次のコマンドを実行してアプリケーションを設定します。

   ```bash
   # Source the Vitis runtime environment
   export LAB_WORK_DIR=<Downloaded Github repository>/Hardware_Accelerators/Design_Tutorials/02-bloom
   ```

2. C アプリケーションをビルドします。

   1. `cpu_src` ディレクトリに移動します。

   2. 次のコマンドを使用して、ドキュメント数を引数として指定して元の C アプリケーションを実行し、比較のためのゴールデン出力ファイルを生成します。

      ```bash
      cd $LAB_WORK_DIR/cpu_src/
      make run
      ```

      生成された出力スコアは、ホスト コードの `cpu_profile_score` 配列に格納されます。この配列は、指定されたドキュメント総数の出力を表します。結果は、次のようになります。

      ```
      ./host 100000
      Initializing data
      Creating documents - total size : 1398.903 MBytes (349725824 words)
      Creating profile weights

      Total execution time of CPU          |  2949.3867 ms
      Compute Hash processing time         |  2569.3266 ms
      Compute Score processing time        |   380.0601 ms
      --------------------------------------------------------------------
      Execution COMPLETE
      ```

3. FPGA 上でアプリケーションを実行します。この演習では、FPGA アクセラレータは並列係数 8 でインプリメントされています。

   * 8 個の入力ワードが並列処理され、各クロック サイクルで 8 つの出力フラグが生成されます。

     最適化されたアプリケーションを FPGA 上で実行するには、次の `make` コマンドを実行します。

     ```bash
     make run_fpga SOLUTION=1
     ```

     次の出力が表示されます。

     ```
     Processing 1398.905 MBytes of data
     Splitting data in 8 sub-buffers of 174.863 MBytes for FPGA processing
     --------------------------------------------------------------------
     Executed FPGA accelerated version  |   427.1341 ms   ( FPGA 230.345 ms )
     Executed Software-Only version     |   3057.6307 ms
     --------------------------------------------------------------------
     Verification: PASS
     ```

     スループットは、次のように計算されます。

     スループット = 合計データ量/合計時間 = 1.39 GB/427.1341 ms = 3.25 GB/s

     FPGA アクセラレーションを効果的に利用することにより、アプリケーションのスループットが 7 倍になりました。

## 次の手順

この手順では、FPGA を使用して達成可能なアクセラレーションを観察しました。次に、[アクセラレーションのためのアプリケーションを構築](./3_architect-the-application.md)し、元のアプリケーションをプロファイリングしてどの関数をアクセラレーションできるかを調べます。希望のアクセラレーションを達成するためのインターフェイス境界およびパフォーマンス制約も定義します。

<hr/>
<p align= center class="sphinxhide"><b><a href="../../../README.md">メイン ページに戻る</a> &mdash; <a href="../../README.md/">ハードウェア アクセラレータ チュートリアルの初めに戻る</a></b></p></br><p align="center" class="sphinxhide"><sup>Copyright&copy; 2020 Xilinx</sup></p>
<p align="center"><sup>この資料は 2021 年 2 月 8 日時点の表記バージョンの英語版を翻訳したもので、内容に相違が生じる場合には原文を優先します。資料によっては英語版の更新に対応していないものがあります。
日本語版は参考用としてご使用の上、最新情報につきましては、必ず最新英語版をご参照ください。</sup></p>
