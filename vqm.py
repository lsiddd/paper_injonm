import numpy as np
import matplotlib as mpl

mpl.use('agg')

import matplotlib.pyplot as plt

ssim_a2a4 = [0.73106
,0.74128
,0.73886
,0.68481
,0.72806
,0.72619
,0.74300
,0.70717
,0.72216
,0.74299
,0.70201
,0.69803
,0.71783
,0.55849
,0.73537
,0.70688
,0.73339
,0.55947
,0.73042
,0.71595
,0.55142
,0.71147
,0.56541
,0.74969
,0.55538
,0.73509
,0.55950
,0.54859
,0.71125
,0.72970
,0.54974
,0.74077
,0.68284
,0.55618
,0.72478
,0.54682
,0.73687
,0.54770
,0.73766
,0.72163
,0.72815
,0.73175
,0.73864
,0.72666
,0.72411
,0.71327
,0.69364
,0.73509
,0.55663
,0.73020
,0.73886
,0.72811
,0.72632
,0.74139
,0.74152
,0.54849
,0.72947
,0.68475
,0.72533
,0.72351
,0.69949
,0.73708
,0.74340
,0.73526
,0.72170
,0.73229
,0.55133
,0.71568
,0.56092
,0.74359
,0.72428
,0.72305
,0.74088
,0.72306
,0.71087
,0.71259
,0.73291
,0.73574
,0.72616
,0.73617
,0.74389
,0.75006
,0.55061
,0.55509
,0.74092
,0.72414
,0.70482
,0.68343
,0.55797
,0.56287
,0.72607
,0.67881
,0.68499
,0.74060
,0.69239
,0.55353
,0.55216
,0.55755
,0.67929
,0.74218
,0.69595
,0.73668
,0.74183
,0.72882
,0.72486
,0.55757
,0.73567
,0.73340
,0.72585
,0.55488
,0.55760
,0.55591
,0.73074
,0.71975
,0.55709
,0.74206
,0.55111
,0.55265
,0.55708
,0.72132
,0.71102
,0.55091
,0.72936
,0.72034
,0.73953
,0.55570
,0.72225
,0.68601
,0.74221
,0.55916
,0.71748
,0.55787
,0.55621
,0.73622
,0.71280
,0.56105
,0.71242
,0.73093
,0.71787
,0.65950
,0.55440
,0.55207
,0.55535
,0.73258
,0.70391
,0.54692
,0.55170
,0.68405
,0.73949
,0.69537
,0.71194
,0.74534
,0.71985
,0.68822
,0.73741
,0.71329
,0.70428
,0.55277
,0.67485
,0.66452
,0.72638
,0.71634
,0.72210
,0.74855
,0.54907
,0.73301
,0.71519
,0.55024
,0.70704
,0.72260
,0.69689
,0.67720
,0.68789
,0.73137
,0.72986
,0.70954
,0.69235
,0.70743
,0.69950
,0.74764
,0.74475
,0.55080
,0.69560
,0.75082
,0.74453
,0.72314
,0.55767
,0.69482
,0.73711
,0.70262
,0.55862
,0.70913
,0.69576
,0.68349
,0.70356
,0.70898
,0.69641
,0.71921
,0.74828
,0.69939
,0.71417
,0.55788
,0.71314
,0.71875
,0.55506
,0.72471
,0.75127
,0.72934
,0.70771
,0.72605
,0.69700
,0.68723
,0.70991
,0.71381
,0.74524
,0.54804
,0.55456
,0.74543
,0.67366
,0.71079
,0.68707
,0.54822
,0.71032
,0.72026
,0.67768
,0.72740
,0.69841
,0.55211
,0.54868
,0.74157
,0.68636
,0.71756
,0.72908
,0.72426
,0.68493
,0.55193
,0.68954
,0.55605
,0.73983
,0.74590
,0.72270
,0.69249
,0.68023
,0.55083
,0.74240
,0.68833
,0.69756
,0.71207
,0.75263
,0.73098
,0.72021
,0.67925
,0.74135
,0.68997
,0.74353
,0.69816
,0.67169
,0.72957
,0.55669
,0.54970
,0.70703
,0.67176
,0.66616
,0.54957
,0.69282
,0.68767
,0.69393
,0.70123
,0.71932
,0.69467
,0.65388
,0.72091
,0.73269
,0.55311
,0.71235
,0.66322
,0.73784
,0.55733
,0.73688
,0.68168
,0.72848
,0.68387
,0.72476
,0.69450
,0.69202
,0.71563
,0.73237
,0.72709
,0.72279
,0.74426
,0.71666
,0.67441
,0.72685
,0.55490
,0.71086
,0.67316
,0.72649
,0.74397
,0.70115
,0.74027
,0.72083
,0.72518
,0.69754
,0.55873
,0.74688
,0.73826
,0.70252
,0.68913
,0.72786
,0.74135
,0.74007
,0.55384
,0.72131
,0.73381
,0.74144
,0.74619
,0.71702
,0.74237
,0.73688
,0.54951
,0.70119
,0.69897
,0.70631
,0.73888
,0.72982
,0.72556
,0.55645
,0.74694
,0.71523
,0.71626
,0.54837
,0.72397
,0.70246
,0.71766
,0.68240
,0.72375
,0.73097
,0.55360
,0.72126
,0.69413
,0.65611
,0.73725
,0.69522
,0.71612
,0.69030
,0.73896
,0.73560
,0.70598
,0.71472
,0.55270
,0.69335
,0.72253
,0.73685
,0.74243
,0.69270
,0.71195
,0.73579
,0.70829
,0.73384
,0.71700
,0.71992
,0.70207
,0.73010
,0.54929
,0.72459
,0.71642
,0.71212
,0.69752
,0.69881
,0.73686
,0.73416
,0.73790
,0.68880
,0.55426
,0.72548
,0.55371
,0.55347
,0.55432
,0.71846
,0.73331
,0.71105
,0.74087
,0.71523
,0.71740
,0.73678
,0.74027
,0.69229
,0.72161
,0.68180
,0.73448
,0.73936
,0.71877
,0.55311
,0.73591
,0.73883
,0.73139
,0.55352
,0.74774
,0.70309
,0.70196
,0.73916
,0.73552
,0.68592
,0.69971
,0.74285
,0.55567
,0.72154
,0.70898
,0.74398
,0.73295
,0.68117
,0.72900
,0.72276
,0.71706
,0.55519
,0.71276
,0.54705
,0.70665
,0.73181
]
ssim_a3 = [0.73649
,0.55507
,0.68411
,0.74324
,0.75345
,0.71902
,0.71861
,0.73997
,0.70597
,0.73777
,0.72964
,0.71889
,0.73101
,0.72278
,0.73099
,0.68276
,0.75200
,0.55439
,0.71862
,0.73877
,0.73035
,0.72443
,0.71293
,0.74735
,0.72434
,0.70638
,0.72565
,0.73999
,0.73822
,0.74681
,0.65674
,0.55346
,0.67973
,0.71202
,0.70848
,0.74145
,0.73585
,0.71743
,0.72804
,0.73335
,0.67631
,0.74229
,0.73921
,0.68347
,0.74857
,0.73795
,0.69013
,0.71585
,0.72246
,0.55270
,0.74429
,0.73885
,0.73713
,0.72517
,0.74540
,0.73754
,0.73691
,0.73506
,0.73583
,0.70300
,0.73748
,0.73585
,0.71972
,0.74199
,0.55415
,0.69886
,0.55626
,0.72514
,0.70660
,0.72293
,0.71953
,0.73339
,0.70414
,0.73971
,0.69949
,0.54756
,0.72885
,0.74821
,0.73226
,0.55252
,0.67637
,0.72112
,0.70434
,0.70412
,0.75103
,0.69066
,0.55319
,0.72345
,0.55895
,0.75186
,0.73218
,0.70256
,0.55708
,0.72068
,0.71821
,0.74042
,0.67216
,0.67292
,0.55341
,0.55389
,0.67608
,0.68798
,0.70524
,0.54631
,0.72845
,0.70193
,0.73065
,0.70227
,0.73687
,0.72749
,0.73331
,0.72319
,0.71246
,0.71738
,0.55262
,0.55608
,0.72917
,0.73522
,0.55661
,0.73018
,0.73625
,0.70296
,0.73415
,0.73577
,0.70929
,0.74536
,0.71874
,0.73347
,0.71784
,0.70322
,0.73644
,0.73834
,0.72445
,0.55775
,0.73872
,0.69356
,0.74186
,0.72165
,0.56000
,0.55413
,0.71215
,0.73394
,0.54588
,0.73313
,0.71560
,0.55324
,0.74138
,0.72533
,0.71135
,0.71819
,0.72395
,0.74282
,0.55456
,0.73719
,0.73745
,0.70183
,0.55812
,0.73838
,0.72242
,0.68092
,0.70903
,0.70435
,0.72186
,0.68664
,0.71583
,0.72453
,0.71428
,0.73077
,0.54981
,0.73557
,0.55896
,0.72795
,0.71768
,0.73844
,0.72451
,0.74724
,0.71856
,0.73726
,0.74583
,0.74572
,0.71633
,0.72525
,0.54842
,0.73451
,0.74904
,0.73917
,0.55657
,0.73758
,0.70155
,0.74007
,0.72012
,0.70872
,0.71946
,0.74593
,0.74422
,0.70233
,0.69515
,0.72438
,0.74050
,0.72082
,0.73105
,0.55596
,0.72968
,0.72460
,0.56022
,0.73691
,0.72004
,0.73572
,0.70599
,0.70631
,0.73300
,0.71760
,0.72775
,0.73445
,0.67760
,0.73872
,0.72755
,0.70478
,0.74415
,0.54683
,0.73130
,0.70350
,0.72671
,0.55709
,0.74519
,0.67623
,0.72028
,0.74031
,0.73601
,0.55513
,0.71906
,0.72320
,0.68752
,0.69679
,0.54702
,0.73932
,0.71628
,0.74144
,0.55560
,0.55802
,0.55189
,0.72932
,0.71384
,0.74212
,0.71986
,0.69513
,0.71360
,0.74919
,0.73959
,0.73288
,0.72306
,0.74246
,0.73211
,0.68383
,0.74466
,0.71907
,0.73714
,0.73008
,0.71486
,0.73657
,0.72064
,0.74231
,0.56155
,0.72893
,0.70588
,0.70077
,0.71488
,0.69590
,0.69688
,0.55239
,0.55110
,0.73982
,0.72709
,0.72405
,0.73179
,0.70566
,0.71150
,0.70328
,0.74143
,0.56295
,0.70723
,0.74874
,0.54768
,0.55428
,0.71250
,0.73231
,0.73593
,0.74201
,0.70743
,0.74231
,0.72479
,0.68972
,0.74531
,0.67351
,0.73913
,0.70082
,0.71751
,0.69956
,0.55202
,0.73318
,0.73465
,0.71484
,0.55342
,0.72442
,0.70984
,0.70815
,0.72544
,0.68563
,0.73583
,0.74237
,0.66654
,0.73663
,0.54991
,0.74425
,0.73292
,0.74762
,0.71506
,0.73392
,0.73179
,0.73679
,0.71878
,0.73269
,0.73717
,0.55961
,0.71193
,0.55450
,0.70621
,0.55531
,0.71938
,0.69722
,0.73863
,0.72505
,0.72494
,0.72183
,0.72447
,0.70789
,0.55716
,0.71531
,0.70137
,0.54775
,0.73687
,0.71731
,0.54963
,0.55355
,0.70408
,0.72554
,0.56200
,0.55692
,0.72829
,0.73705
,0.72296
,0.73777
,0.55531
,0.55297
,0.55895
,0.73126
,0.69622
,0.72036
,0.73299
,0.55340
,0.70649
,0.73810
,0.73387
,0.72251
,0.68974
,0.72997
,0.70336
,0.70815
,0.69328
,0.71299
,0.55993
,0.75077
,0.66888
,0.74133
,0.55733
,0.69926
,0.73924
,0.72205
,0.71056
,0.72166
,0.73173
,0.70905
,0.55651
,0.72421
,0.71978
,0.72927
,0.69694
,0.69621
,0.70138
,0.55601
,0.71812
,0.71936
,0.67408
,0.70793
,0.72373
,0.55120
,0.55805
,0.73078
,0.70840
,0.70547
,0.54780
,0.74318
,0.71803
,0.71468
,0.54746
,0.69546
,0.69133
,0.72759
,0.55503
,0.72187
,0.55724
,0.55573
,0.72193
,0.71857
,0.69002
,0.74365
,0.55029
,0.71781
,0.55873
,0.70801
,0.71565
,0.72202
,0.72964]
ssim_ahp = [0.71754
,0.95603
,0.93658
,0.54060
,0.70619
,0.67364
,0.95983
,0.52447
,0.58488
,0.53264
,0.79219
,0.78286
,0.78028
,0.96946
,0.95971
,0.92059
,0.78363
,0.95541
,0.73774
,0.95680
,0.78587
,0.94097
,0.95168
,0.58765
,0.95970
,0.78185
,0.60725
,0.95405
,0.97544
,0.96141
,0.96704
,0.98648
,0.96985
,0.73097
,0.96254
,0.78230
,0.94251
,0.74557
,0.96446
,0.68527
,0.89826
,0.70754
,0.98238
,0.95144
,0.73259
,0.96602
,0.73838
,0.97444
,0.96142
,0.97027
,0.98617
,0.78404
,0.94221
,0.61108
,0.72909
,0.78859
,0.97973
,0.75105
,0.96820
,0.96104
,0.96335
,0.65347
,0.98121
,0.68101
,0.90342
,0.61105
,0.94505
,0.97825
,0.69151
,0.93441
,0.71462
,0.96563
,0.96968
,0.95215
,0.90969
,0.96938
,0.90590
,0.78770
,0.78448
,0.96545
,0.66019
,0.94189
,0.94977
,0.95382
,0.56226
,0.95196
,0.57979
,0.77352
,0.97888
,0.93906
,0.78327
,0.68498
,0.90563
,0.95624
,0.97715
,0.95656
,0.95021
,0.97999
,0.78308
,0.94972
,0.95278
,0.92718
,0.79304
,0.97124
,0.96923
,0.72124
,0.77986
,0.97729
,0.97373
,0.68441
,0.90625
,0.72467
,0.97041
,0.65173
,0.93167
,0.76527
,0.67187
,0.78426
,0.66699
,0.98512
,0.96758
,0.97356
,0.95939
,0.96890
,0.74901
,0.94423
,0.93937
,0.78610
,0.98442
,0.67036
,0.95962
,0.59021
,0.67955
,0.95565
,0.92978
,0.65827
,0.94506
,0.65313
,0.92244
,0.72214
,0.71392
,0.97025]
print (np.mean(ssim_ahp))
print (np.mean(ssim_a2a4))
print (np.mean(ssim_a3))

data_to_plot = [ssim_ahp, ssim_a2a4, ssim_a3]

fig = plt.figure(1, figsize=(9, 6))
plt.title("SSIM")
ax = fig.add_subplot(111)

bp = ax.boxplot(data_to_plot)

bp = ax.boxplot(data_to_plot, patch_artist=True)

for box in bp['boxes']:
    box.set( color='#7570b3', linewidth=2)
    box.set( facecolor = '#1b9e77' )

for whisker in bp['whiskers']:
    whisker.set(color='#7570b3', linewidth=2)

for cap in bp['caps']:
    cap.set(color='#7570b3', linewidth=2)

for median in bp['medians']:
    median.set(color='#b2df8a', linewidth=2)

for flier in bp['fliers']:
    flier.set(marker='o', color='#e7298a', alpha=0.5)

ax.set_xticklabels(['AHP', 'A2-A4-RSRQ', 'A3-RSRP'])
plt.show()
fig.savefig('fig1-ssim.png', bbox_inches='tight')
