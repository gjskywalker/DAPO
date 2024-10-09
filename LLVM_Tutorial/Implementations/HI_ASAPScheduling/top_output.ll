; ModuleID = 'top_output.bc'
source_filename = "2dloop2darray_pl_brAccess_a.cc"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: argmemonly mustprogress nofree nosync nounwind uwtable
define dso_local void @_Z1fPA100_i(ptr nocapture noundef %A) local_unnamed_addr #0 {
entry:
  %A1 = ptrtoint ptr %A to i64
  %B = alloca [50 x [100 x i32]], align 16
  %B4 = ptrtoint ptr %B to i64
  call void @llvm.lifetime.start.p0(i64 20000, ptr nonnull %B) #2
  br label %for.cond1.preheader

for.cond1.preheader:                              ; preds = %for.cond.cleanup3, %entry
  %lsr.iv11 = phi i64 [ %lsr.iv.next12, %for.cond.cleanup3 ], [ %B4, %entry ]
  %HI.indvars.iv76new1 = phi i7 [ 0, %entry ], [ %HI.indvars.iv.next77new2, %for.cond.cleanup3 ]
  br label %for.body4

for.cond.cleanup3:                                ; preds = %for.body4
  %HI.indvars.iv.next77new2 = add i7 %HI.indvars.iv76new1, 1
  %lsr.iv.next12 = add i64 %lsr.iv11, 1
  %HI.exitcond79.notnew3 = icmp eq i7 %HI.indvars.iv.next77new2, -28
  br i1 %HI.exitcond79.notnew3, label %for.cond16.preheader.preheader, label %for.cond1.preheader, !llvm.loop !5

for.cond16.preheader.preheader:                   ; preds = %for.cond.cleanup3
  br label %for.cond16.preheader

for.body4:                                        ; preds = %for.body4, %for.cond1.preheader
  %lsr.iv17 = phi i32 [ %lsr.iv.next18, %for.body4 ], [ 0, %for.cond1.preheader ]
  %HI.lsr.iv15new4 = phi i6 [ %HI.lsr.iv.next16new5, %for.body4 ], [ -14, %for.cond1.preheader ]
  %lsr.iv13 = phi i64 [ %lsr.iv.next14, %for.body4 ], [ %lsr.iv11, %for.cond1.preheader ]
  %0 = inttoptr i64 %lsr.iv13 to ptr
  store i32 %lsr.iv17, ptr %0, align 4, !tbaa !8
  %lsr.iv.next14 = add i64 %lsr.iv13, 100
  %HI.lsr.iv.next16new5 = add i6 %HI.lsr.iv15new4, -1
  %lsr.iv.next18 = add nuw nsw i32 %lsr.iv17, 1
  %HI.exitcond.notnew6 = icmp eq i6 %HI.lsr.iv.next16new5, 0
  br i1 %HI.exitcond.notnew6, label %for.cond.cleanup3, label %for.body4, !llvm.loop !12

for.cond16.preheader:                             ; preds = %for.cond.cleanup18, %for.cond16.preheader.preheader
  %lsr.iv5 = phi i64 [ %B4, %for.cond16.preheader.preheader ], [ %lsr.iv.next6, %for.cond.cleanup18 ]
  %lsr.iv = phi i64 [ %A1, %for.cond16.preheader.preheader ], [ %lsr.iv.next, %for.cond.cleanup18 ]
  %HI.indvars.iv86new7 = phi i7 [ %HI.indvars.iv.next87new8, %for.cond.cleanup18 ], [ 0, %for.cond16.preheader.preheader ]
  br label %for.body19

for.cond.cleanup13:                               ; preds = %for.cond.cleanup18
  call void @llvm.lifetime.end.p0(i64 20000, ptr nonnull %B) #2
  ret void

for.cond.cleanup18:                               ; preds = %for.body19
  %HI.indvars.iv.next87new8 = add i7 %HI.indvars.iv86new7, 1
  %lsr.iv.next = add i64 %lsr.iv, 1
  %lsr.iv.next6 = add i64 %lsr.iv5, 1
  %HI.exitcond89.notnew9 = icmp eq i7 %HI.indvars.iv.next87new8, -28
  br i1 %HI.exitcond89.notnew9, label %for.cond.cleanup13, label %for.cond16.preheader, !llvm.loop !13

for.body19:                                       ; preds = %for.body19, %for.cond16.preheader
  %HI.lsr.iv9new10 = phi i6 [ %HI.lsr.iv.next10new11, %for.body19 ], [ -16, %for.cond16.preheader ]
  %lsr.iv7 = phi i64 [ %lsr.iv.next8, %for.body19 ], [ %lsr.iv5, %for.cond16.preheader ]
  %lsr.iv2 = phi i64 [ %1, %for.body19 ], [ %lsr.iv, %for.cond16.preheader ]
  %1 = add i64 %lsr.iv2, 100
  %2 = inttoptr i64 %1 to ptr
  %3 = load i32, ptr %2, align 4, !tbaa !8
  %mul = mul nsw i32 %3, %3
  %4 = inttoptr i64 %lsr.iv2 to ptr
  %5 = load i32, ptr %4, align 4, !tbaa !8
  %add = add nsw i32 %mul, %5
  %6 = add i64 %lsr.iv7, 200
  %7 = inttoptr i64 %6 to ptr
  %8 = load i32, ptr %7, align 4, !tbaa !8
  %add38 = add nsw i32 %add, %8
  %9 = add i64 %lsr.iv2, 200
  %10 = inttoptr i64 %9 to ptr
  store i32 %add38, ptr %10, align 4, !tbaa !8
  %lsr.iv.next8 = add i64 %lsr.iv7, 100
  %HI.lsr.iv.next10new11 = add i6 %HI.lsr.iv9new10, -1
  %HI.exitcond85.notnew12 = icmp eq i6 %HI.lsr.iv.next10new11, 0
  br i1 %HI.exitcond85.notnew12, label %for.cond.cleanup18, label %for.body19, !llvm.loop !14
}

; Function Attrs: argmemonly nocallback nofree nosync nounwind willreturn
declare void @llvm.lifetime.start.p0(i64 immarg, ptr nocapture) #1

; Function Attrs: argmemonly nocallback nofree nosync nounwind willreturn
declare void @llvm.lifetime.end.p0(i64 immarg, ptr nocapture) #1

attributes #0 = { argmemonly mustprogress nofree nosync nounwind uwtable "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { argmemonly nocallback nofree nosync nounwind willreturn }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{!"clang version 16.0.0 (git@github.com:llvm/llvm-project.git 53c0be28a704670b37d816ecdb91c637d5423ef1)"}
!5 = distinct !{!5, !6, !7}
!6 = !{!"llvm.loop.mustprogress"}
!7 = !{!"llvm.loop.unroll.disable"}
!8 = !{!9, !9, i64 0}
!9 = !{!"int", !10, i64 0}
!10 = !{!"omnipotent char", !11, i64 0}
!11 = !{!"Simple C++ TBAA"}
!12 = distinct !{!12, !6, !7}
!13 = distinct !{!13, !6, !7}
!14 = distinct !{!14, !6, !7}
