; ModuleID = 'top1.bc'
source_filename = "test.cc"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local noundef i32 @_Z3addiiii(i32 noundef %x, i32 noundef %y, i32 noundef %z, i32 noundef %w) #0 {
entry:
  %x.addr = alloca i32, align 4
  %y.addr = alloca i32, align 4
  %z.addr = alloca i32, align 4
  %w.addr = alloca i32, align 4
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 %x, ptr %x.addr, align 4
  store i32 %y, ptr %y.addr, align 4
  store i32 %z, ptr %z.addr, align 4
  store i32 %w, ptr %w.addr, align 4
  %0 = load i32, ptr %x.addr, align 4
  %1 = load i32, ptr %y.addr, align 4
  %mul = mul nsw i32 %0, %1
  %2 = load i32, ptr %z.addr, align 4
  %add = add nsw i32 %mul, %2
  store i32 %add, ptr %a, align 4
  %3 = load i32, ptr %x.addr, align 4
  %4 = load i32, ptr %y.addr, align 4
  %mul1 = mul nsw i32 %3, %4
  %5 = load i32, ptr %w.addr, align 4
  %mul2 = mul nsw i32 %mul1, %5
  store i32 %mul2, ptr %b, align 4
  %6 = load i32, ptr %a, align 4
  %add3 = add nsw i32 %6, %mul2
  ret i32 %add3
}

attributes #0 = { mustprogress noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 18.0.0 (git@github.com:llvm/llvm-project.git bde5717d4638c27614d9d4a2e53df27087a69841)"}
