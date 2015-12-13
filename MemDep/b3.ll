; ModuleID = 'b3.bc'
target datalayout = "e-m:e-p:32:32-f64:32:64-f80:32-n8:16:32-S128"
target triple = "i686-pc-linux-gnu"

@B = common global [100 x i32] zeroinitializer, align 4
@A = common global [100 x i32] zeroinitializer, align 4
@C = common global [100 x i32] zeroinitializer, align 4

; Function Attrs: nounwind
define void @loop() #0 {
entry:
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, i32* %i, align 4
  %cmp = icmp slt i32 %0, 100
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %1 = load i32, i32* %i, align 4
  %2 = load i32, i32* %i, align 4
  %arrayidx = getelementptr inbounds [100 x i32], [100 x i32]* @B, i32 0, i32 %2
  store i32 %1, i32* %arrayidx, align 4
  %3 = load i32, i32* %i, align 4
  %arrayidx1 = getelementptr inbounds [100 x i32], [100 x i32]* @A, i32 0, i32 %3
  store i32 %1, i32* %arrayidx1, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %4 = load i32, i32* %i, align 4
  %inc = add nsw i32 %4, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  store i32 0, i32* %i, align 4
  br label %for.cond.2

for.cond.2:                                       ; preds = %for.inc.8, %for.end
  %5 = load i32, i32* %i, align 4
  %cmp3 = icmp slt i32 %5, 100
  br i1 %cmp3, label %for.body.4, label %for.end.10

for.body.4:                                       ; preds = %for.cond.2
  %6 = load i32, i32* %i, align 4
  %arrayidx5 = getelementptr inbounds [100 x i32], [100 x i32]* @A, i32 0, i32 %6
  %7 = load i32, i32* %arrayidx5, align 4
  %8 = load i32, i32* %i, align 4
  %arrayidx6 = getelementptr inbounds [100 x i32], [100 x i32]* @B, i32 0, i32 %8
  %9 = load i32, i32* %arrayidx6, align 4
  %add = add nsw i32 %7, %9
  %10 = load i32, i32* %i, align 4
  %arrayidx7 = getelementptr inbounds [100 x i32], [100 x i32]* @C, i32 0, i32 %10
  store i32 %add, i32* %arrayidx7, align 4
  br label %for.inc.8

for.inc.8:                                        ; preds = %for.body.4
  %11 = load i32, i32* %i, align 4
  %inc9 = add nsw i32 %11, 1
  store i32 %inc9, i32* %i, align 4
  br label %for.cond.2

for.end.10:                                       ; preds = %for.cond.2
  ret void
}

attributes #0 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.7.0 (tags/RELEASE_370/final)"}
