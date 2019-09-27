#! /bin/bash
scpars fft.cpp

bdltran  -EE -c2000 -s -Zresource_fcnt=GENERATE -Zname_sensitive=YES -Zresource_mcnt=GENERATE -Zdup_reset=YES -Zfolding_sharing=inter_stage -tcio -OIF -EE -lb /eda/cwb/cyber_611/LINUX/packages/asic_45.BLIB -lfl /eda/cwb/cyber_611/LINUX/packages/asic_45.FLIB fft.IFF

veriloggen fft_E.IFF
