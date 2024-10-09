; ModuleID = '2dloop1darray_notpl.cc'
source_filename = "2dloop1darray_notpl.cc"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: mustprogress nofree norecurse nosync nounwind memory(argmem: readwrite) uwtable
define dso_local void @_Z1fPi(ptr nocapture noundef %A) local_unnamed_addr #0 {
entry:
  %arrayidx = getelementptr inbounds i32, ptr %A, i64 3
  %arrayidx1 = getelementptr inbounds i32, ptr %A, i64 4
  %0 = load i32, ptr %arrayidx1, align 4, !tbaa !5
  %cmp2 = icmp eq i32 %0, 8
  %arrayidx3 = getelementptr inbounds i32, ptr %A, i64 9
  %arrayidx4 = getelementptr inbounds i32, ptr %A, i64 6
  %invariant.gep = getelementptr i32, ptr %A, i64 8
  br label %for.body

for.cond.cleanup:                                 ; preds = %for.cond.cleanup7
  ret void

for.body:                                         ; preds = %entry, %for.cond.cleanup7
  %j.027 = phi i32 [ 0, %entry ], [ %inc15, %for.cond.cleanup7 ]
  store i32 6, ptr %arrayidx, align 4, !tbaa !5
  br i1 %cmp2, label %if.then, label %for.body8.preheader

for.body8.preheader:                              ; preds = %if.then, %for.body
  br label %for.body8

if.then:                                          ; preds = %for.body
  %1 = load i32, ptr %arrayidx3, align 4, !tbaa !5
  %add = add nsw i32 %1, 3
  store i32 %add, ptr %arrayidx4, align 4, !tbaa !5
  br label %for.body8.preheader

for.cond.cleanup7:                                ; preds = %for.body8
  %inc15 = add nuw nsw i32 %j.027, 1
  %exitcond30.not = icmp eq i32 %inc15, 100
  br i1 %exitcond30.not, label %for.cond.cleanup, label %for.body, !llvm.loop !9

for.body8:                                        ; preds = %for.body8.preheader, %for.body8
  %indvars.iv = phi i64 [ %indvars.iv.next, %for.body8 ], [ 0, %for.body8.preheader ]
  %arrayidx9 = getelementptr inbounds i32, ptr %A, i64 %indvars.iv
  %2 = load i32, ptr %arrayidx9, align 4, !tbaa !5
  %add10 = add nsw i32 %2, 1
  %gep = getelementptr i32, ptr %invariant.gep, i64 %indvars.iv
  store i32 %add10, ptr %gep, align 4, !tbaa !5
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  %exitcond.not = icmp eq i64 %indvars.iv.next, 50
  br i1 %exitcond.not, label %for.cond.cleanup7, label %for.body8, !llvm.loop !12
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind memory(argmem: readwrite) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{!"clang version 18.0.0 (git@github.com:llvm/llvm-project.git bde5717d4638c27614d9d4a2e53df27087a69841)"}
!5 = !{!6, !6, i64 0}
!6 = !{!"int", !7, i64 0}
!7 = !{!"omnipotent char", !8, i64 0}
!8 = !{!"Simple C++ TBAA"}
!9 = distinct !{!9, !10, !11}
!10 = !{!"llvm.loop.mustprogress"}
!11 = !{!"llvm.loop.unroll.disable"}
!12 = distinct !{!12, !10, !11}
