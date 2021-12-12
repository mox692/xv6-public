* 11/7
  * makefile読んで、buildの構成をざっくり把握した
    * 構造としては、
      * kernel
      * bootloader
      * fs
    * みたいな構成になってそう
* next
  * bootloader(bootasm.S bootmain.c)あたりからのんびり読んでいく.
    * ざっと見た感じ、legacy bios(16bit mode)っぽいw

* 11/9
  * bootasm, bootmain読んだ
  * 次からkernel読めそう?
    * entry.Sとかいうのもあるから、先にそっち？
    * もっかいmakefile見てbuildの手順確認する.

* 11/11
  * main.cに入った瞬間、むずすぎた..
    * この辺はblogとかを読みつつ進めた方が良さげ.
  * linker scriptをざっくり読んだ(ここはそこまでむずくなかった.)

* 11/23
  * mpinit, lapicinit辺りをざっくり読んだ.
    * apicに関しては https://ja.wikipedia.org/wiki/APIC がわかりやすい

* 11/23
  * gdtについて復習した
    * mikan本
  * apic関連いろいろ読みすぎた
      * lapicinit
      * picinit
      * ioapicinit
    * が理解できれば良さそう
    * あと、仕様書を見つけて、そこから必要な情報を取ってくるskillをあげたい.

* 11/24
  * 「必要な情報を仕様書から引っ張ってこれるようになりたい」ってことで、試しにtimer関連のハードの設定を仕様書から引っ張って読んでみた
    * lapic.cのはじめの方

* 11/25
  * uartがむずすぎる
    * というか前提知識がなさすぎて

* 11/28
  * userinitでuser procが生成される処理をみて行った.
    * initcode.S
    * int T_SYSCALL -> trap.cで定義されたvectors[]の各エントリに飛ぶ
    * vectors.Sはvectors.plから生成されてて、これはtrapasm.Sのalltrapsを読んでいる
    * alltrapsはtrap()を呼ぶ
    * trap.cは各割り込みの種類に応じて、処理を行う.
    * まとめ:
      * interrupt or syscallが発生.この割り込み時、idtエントリの情報も保持してる.この処理はハードウェア的に処理される
      * tvinit()で設定した関数へ飛ぶ
  * 残る疑問
    * trapから帰ってくる時の処理は？
      * syscall()してから、戻ってくる処理がパッとみ見当たらない
  * 参考
    * https://www.utam0k.jp/blog/2019/07/08/xv6_traps_interrupts_drivers_1/

* 11/29
  * bufferは何に使うん？
  * initでlockをとりあえずとるだけ、みたいなのが多いけど、これは何？
  * spinlock.cのaquire , releaeseも関係してるのかも.

* 12/2
  * user processの起動の流れを見る 
    * tvinitでidtに割り込みが登録される
      * ここでsyscallは64ってのが決められてる
    * idtinitで割り込みが入るようになる

    * inituvmでinitCodeをpageの先頭に置いている
      * その他細かいpageのmapなどもしてる.
    * proc.cでeipを0に設定している
    * makefileでinitcode.Sが0番地からstartするとして定義されている

  * 起動の流れはちょっとイメージできた
  * 次はscehdulingの部分をみたい。
    * なんやかんやここを理解するのに、過去のコードに戻りそう.

* 12/7
  * elfのi386上で実行してるみたいだけど、ldだけのoptionでi386のバイラリができるものなの？
  * コンパイラにもoptionを付けないといけないと思ってた.
  * ここ(https://www.utam0k.jp/blog/2019/12/20/xv6_scheduling_1/)を読みつつ、schedの大枠理解に努めた
    * タイマー割り込みをトリガとして
      * old -> sched -> new
    * の順番に処理が写っていくみたい 
    * : timer割り込みの機構をみたい.(timer割り込みなんて設定してた?)
      * -> trap.cのtrap()で、trapのハンドラを定義していた
      * ついでにinterruotの復讐もした
    * TODO : timer
  * cpuごとにscheduler()が呼ばれてるみたいだけど、各cpuが協調してscheduler()を呼ぶ仕組みは、xz6では書いてない？
  * https://qiita.com/knknkn1162/items/0bc9afc3ae304590e16c#mpmain%E3%81%AEscheduler 神記事を道桁
  * 次はinit.c, fork関数とexec関数辺りを読む

* 12/12
  * execを少し読んだ。
    * userランドのコードは多少読みやすい
    * ざっくりとした流れはわかるけど、細かいのはちゃんと読んでない
  * schedule周り
    * さしあたり、暫定的に以下の結論で。
      1. userinit()でinitcode.Sをprocessの開始地点とする
      2. scheduler()のswtch()で実際にinitcodeのprocessへ飛ぶ
      3. ~ process実行される ~
      4. timer割り込みがはいる！！
      5. trapハンドラである、trap.cのtrap()のyield();の中の  sched();でschedulerに戻す.
      6. 2に戻る

### 参考文献
* boot, pagingの設定(main.cのはじめあたり)
  * https://yohei.codes/ja/post/xv6-memory-2/
  * 
* init processの流れ
  * https://speakerdeck.com/toasa/xv6-initpurosesu-kotohazime
### 起動
* make -> make qemu
* 停止は