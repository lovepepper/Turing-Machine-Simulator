#Q = {q0,q1,q2,q3,q4,q5,q6,q7,q8,q9,q10,q11,q12,q13,q14,q15,q16,accept,accept1,accept2,accept3,accept4,accept5,halt_accept,reject,reject1,reject2,reject3,reject4,reject5,reject6,halt_reject}

#S = {1,x,=}

#T = {1,x,=,_,#,T,r,u,e,F,a,l,s}

#q0 = q0

#B = _

#F = {halt_accept}

; State q0 to q6: check the format of multiplication expression
q0 1 1 r q1
q0 * * * reject

q1 1 1 r q1
q1 x x r q2
q1 * * * reject

q2 1 1 r q3
q2 * * * reject

q3 1 1 r q3
q3 = = r q4
q3 * * * reject

q4 1 1 r q5
q4 * * * reject

q5 1 1 r q5
q5 _ _ l q6
q5 * * * reject

q6 1 1 l q6
q6 x x l q6
q6 = = l q6
q6 _ _ r q7

; State q7 to q14: in each loop, elimate one '1' before 'x' and n '1's after '='
q7 1 _ r q8
q7 x x r q15

q8 1 1 r q8
q8 x x r q9

q9 # # r q9
q9 1 # r q10
q9 = = l q13

q10 1 1 r q10
q10 = = r q10
q10 _ _ l q11

q11 1 _ l q12
q11 * * * reject

q12 1 1 l q12
q12 = = l q12
q12 # # l q12
q12 x x r q9

q13 # 1 l q13
q13 x x l q14

q14 1 1 l q14
q14 _ _ r q7

; State q15 and q16: check if there are any '1's after '='
q15 1 1 r q15
q15 = = r q16

q16 _ _ l accept
q16 * * * reject

accept * * l accept
accept _ _ r accept1
accept1 * _ r accept1
accept1 _ _ * accept2
accept2 _ T r accept3
accept3 _ r r accept4
accept4 _ u r accept5
accept5 _ e * halt_accept

reject * * l reject
reject _ _ r reject1
reject1 * _ r reject1
reject1 _ _ * reject2
reject2 _ F r reject3
reject3 _ a r reject4
reject4 _ l r reject5
reject5 _ s r reject6
reject6 _ e * halt_reject