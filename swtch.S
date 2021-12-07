# Context switch
#
#   void swtch(struct context **old, struct context *new);
# 
# Save the current registers on the stack, creating
# a struct context, and save its address in *old.
# Switch stacks to new and pop previously-saved registers.

# MEMO: 第一引数: oldなcontext, 第二引数: newなconntext
.globl swtch
swtch:
  movl 4(%esp), %eax
  movl 8(%esp), %edx

  # Save old callee-saved registers
  pushl %ebp
  pushl %ebx
  pushl %esi
  pushl %edi

  # Switch stacks
  movl %esp, (%eax) # eaxにとりあえず戻り先のaddrを積んでおくってこと？？
  movl %edx, %esp # 新しいstack pointerをセット

  # Load new callee-saved registers
  popl %edi
  popl %esi
  popl %ebx
  popl %ebp
  # MEMO: ret先がnew processになる?
  # TODO:https://qiita.com/knknkn1162/items/0bc9afc3ae304590e16c#scheduler%E9%96%A2%E6%95%B0%E3%81%AE%E5%86%8D%E9%96%8B
  # 曰く、ここのretはforkretに飛ぶらしいぞ 
  ret
