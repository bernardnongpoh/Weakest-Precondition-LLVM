define i32 @_Z3maxii(i32 %a, i32 %b) #1 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 %a, i32* %2, align 4
  store i32 %b, i32* %3, align 4
  %4 = load i32, i32* %2, align 4
  %5 = load i32, i32* %3, align 4
  %6 = icmp sgt i32 %4, %5
  br i1 %6, label %7, label %9

; <label>:7                                       ; preds = %0
  %8 = load i32, i32* %2, align 4
  store i32 %8, i32* %1, align 4
  br label %11

; <label>:9                                       ; preds = %0
  %10 = load i32, i32* %3, align 4
  store i32 %10, i32* %1, align 4
  br label %11

; <label>:11                                      ; preds = %9, %7
  %12 = load i32, i32* %1, align 4
  ret i32 %12
}

