import os
import pickle 
import numpy
import collections

opt_passes_str = "annotation2metdadata forceattrs inferattrs coro-early lower-expect simplifycfg sroa early-cse callsite-splitting openmp-opt ipsccp called-value-propagation globalopt typepromotion argpromotion instcombine aggressive-instcombine always-inline inliner-wrapper wholeprogramdevirt module-inline inline rpo-function-attrs openmp-opt-cgscc speculative-execution jump-threading correlated-propagation libcalls-shrinkwrap tailcallelim reassociate constraint-elimination loop-simplify lcssa loop-instsimplify loop-simplifycfg licm loop-rotate simple-loop-unswitch loop-idiom indvars loop-deletion loop-unroll-full vector-combine mldst-motion gvn sccp bdce adce memcpyopt dse move-auto-init coro-elide coro-split coro-cleanup deadargelim  elim-avail-extern recompute-globalsaa float2int lower-constant-intrinsics chr loop-distribute inject-tli-mappings loop-vectorize infer-alignment loop-load-elim slp-vectorizer loop-unroll alignment-from-assumptions loop-sink instsimplify div-rem-pairs constmerge cg-profile rel-lookup-table-converter annotation-remarks verify"

opt_passes_str = tuple(opt_passes_str.split())

prune_result = {}
    
with open("prune_passes_pgm.pkl", "rb") as f:
    prune_result = pickle.load(f)

f = open("prune_result.txt", "w")

passes_len = 0

Embeddings_passes = list()
for i, (a, av) in enumerate(prune_result.items()):
    pass_indice = numpy.zeros(76,)
    # f.write("Current Program:{0:s} -- Cycles:{1:d} -- Passes:".format(a, av['cycle']))
    # f.write(''.join(str(av['passes'])))
    # for j in av['passes']:
    #     f.write(" " + opt_passes_str[j])
    # f.write("\n")
    # passes_len += len(av['passes'])
    for n in av['passes']:
        pass_indice[n] += 1
    Embeddings_passes.append(pass_indice)

with open("Embeddings_passes.pkl", "wb") as f:
    pickle.dump(Embeddings_passes, f)

# f.write("Totally Length for all passes: {1:d} -- Average Length of Passes: {0:4.3f} -- Program Number: {2:d} \n".format(passes_len/i, passes_len, i))
# f.write("Used Passes: {} \n".format(''.join(str(pass_indice))))

# for i, n in enumerate(pass_indice):
#     if n != 0:
#         f.write(opt_passes_str[int(i)] + " ")

# 3 4 13 12 10
'''
final latency : 50
3 : 100
3 4 : 200
3 4 13 : 150
3 4 13 12 : 100
3 4 13 12 10 : 50

3 4 13 10 : 40
'''