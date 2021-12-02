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
    * 

* 11/28
  * userinitでuser procが生成される処理をみて行った.
    * initcode.S
    * int T_SYSCALL -> trap.cで定義されたvectors[]の各エントリに飛ぶ
    * vectors.Sはvectors.plから生成されてて、これはtrapasm.Sのalltrapsを読んでいる
    * alltrapsはtrap()を呼ぶ
    * trap.cは各割り込みの種類に応じて、処理を行う.
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

### 参考文献
* boot, pagingの設定(main.cのはじめあたり)
  * https://yohei.codes/ja/post/xv6-memory-2/
  * 
* init processの流れ
  * https://speakerdeck.com/toasa/xv6-initpurosesu-kotohazime
### 起動
* make -> make qemu
* 停止は