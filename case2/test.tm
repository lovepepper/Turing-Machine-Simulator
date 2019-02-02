#Q = {q0,q1,q2,q3,q4,q5,q6,q7,q8,accept,accept1,accept2,accept3,accept4,accept5,halt_accept,reject,reject1,reject2,reject3,reject4,reject5,reject6,halt_reject}

#S = {a,b}

#T = {a,b,_,X,Y,#,T,r,u,e,F,a,l,s}

#q0 = q0

#B = _

#F = {halt_accept}

; State q0 to q4: check if the length is even and find the middle of input string
q0 _ _ * accept
q0 a X r q1
q0 b Y r q1
q0 X X l q4
q0 Y Y l q4 

q1 a a r q1
q1 b b r q1
q1 * * l q2

q2 a X l q3
q2 b Y l q3
q2 * * * reject

q3 a a l q3
q3 b b l q3
q3 X X r q0
q3 Y Y r q0

q4 X a l q4
q4 Y b l q4
q4 _ _ r q5

; State q5 to q8: match every symbol in the left half and the right half
q5 a _ r q6
q5 b _ r q7
q5 # # * accept

q6 X # l q8
q6 * * r q6
q6 Y Y * reject 

q7 Y # l q8
q7 * * r q7
q7 X X * reject 

q8 * * l q8
q8 _ _ r q5

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