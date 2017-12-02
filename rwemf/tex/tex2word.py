import os
import re
info=''
with open('./RWEMF.tex','r') as f:
    info=f.read()
    info=info.replace('.eps','.png')
    info=info.replace('\\mathop{argmin}','argmin')

    alg1="\\begin{figure}[H]\n\
    \\includegraphics[width=0.45\\paperwidth]{alg1.png}\n\
    \\end{figure}\n"
    alg2="\\begin{figure}[H]\n\
    \\includegraphics[width=0.45\\paperwidth]{alg2.png}\n\
    \\end{figure}\n"

    idx_b=info.find('\\begin{algorithm}',0)
    idx_e=info.find('\\end{algorithm} ',idx_b+1)
    pattern=info[idx_b:idx_e+16]
    info=info.replace(pattern,alg1)

    idx_b=info.find('\\begin{algorithm}',0)
    idx_e=info.find('\\end{algorithm}',idx_b+1)
    pattern=info[idx_b:idx_e+16]
    info=info.replace(pattern,alg2)

with open('./RWEMF_2word.tex','w') as f:
    f.write(info)

output = os.popen('pandoc -s RWEMF_2word.tex -o  RWEMF_2word.docx')
print output

output = os.popen('zip RWEMF-20171201.zip ./RWEMF.pdf ./RWEMF_2word.docx')
print output
print "ok"