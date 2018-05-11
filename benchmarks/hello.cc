; ModuleID = '<stdin>'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
  %a = alloca i32, align 4
  store i32 10, i32* %a, align 4
  ret i32 0
}

; Function Attrs: nounwind uwtable
define i32 @hello() #0 {
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 10, i32* %a, align 4
  store i32 0, i32* %b, align 4
  store i32 0, i32* %a, align 4
  br label %1

; <label>:1                                       ; preds = %5, %0
  %2 = load i32, i32* %a, align 4
  %3 = icmp slt i32 %2, 4
  br i1 %3, label %4, label %8

; <label>:4                                       ; preds = %1
  store i32 0, i32* %b, align 4
  br label %5

; <label>:5                                       ; preds = %4
  %6 = load i32, i32* %a, align 4
  %7 = add nsw i32 %6, 1
  store i32 %7, i32* %a, align 4
  br label %1

; <label>:8                                       ; preds = %1
  ret i32 0
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.7.1 (tags/RELEASE_371/final)"}
