<XCom 2.4>
<S1F1 S Are You There
>

<S1F2 P On Line Data 
  <LIST 2 
    <ASCII 4 MDLN>
    <ASCII 6 SOFTREV>
  >
>

<S1F5 S Formatted Status Request
  <BINARY 1 SFCD>
>

<S1F13 S Establish Communications Request
  <LIST 2 
    <ASCII 4 MDLN>
    <ASCII 6 SOFTREV>
  >
>

<S1F14 P Establish Communications Request Acknowledge
  <LIST 2 
    <BINARY 1 COMMACK>
    <LIST 2 
      <ASCII 4 MDLN>
      <ASCII 6 SOFTREV>
    >
  >
>

<S1F17 S Equipment Mode Change Report
  <LIST n 
    <LIST 2 
      <UINT2 n EQPID>
      <UINT2 n STATE>
    >
  >
>

<S1F18 P Equipment Mode Change Acknowledge
  <BINARY 1 ACK6>
>

<S2F17 S Date and Time Request
>

<S2F18 P Data and Time Data
  <ASCII 14 TIME>
>

<S2F41 S Host Command Send
  <LIST 6 
    <UINT1 n RCMD>
    <UINT1 n WORKTYPE>
    <UINT2 n SPORT>
    <UINT2 n EPORT>
    <LIST 2 
      <LIST 4 
        <UINT2 n SLS>
        <UINT2 n SLZ>
        <UINT2 n SLX>
        <UINT2 n SLY>
      >
      <LIST 4 
        <UINT2 n DLS>
        <UINT2 n DLZ>
        <UINT2 n DLX>
        <UINT2 n DLY>
      >
    >
    <LIST 2 
      <LIST 5 
        <ASCII 10 TRAYID>
        <ASCII 20 BATCHNO>
        <UINT1 n TRAYTYPE>
        <UINT1 n EMGJOB>
        <UINT1 n TRAYSIZE>
      >
      <LIST 5 
        <ASCII 10 TRAYID>
        <ASCII 20 BATCHNO>
        <UINT1 n TRAYTYPE>
        <UINT1 n EMGJOB>
        <UINT1 n TRAYSIZE>
      >
    >
  >
>

<S2F42 P Host Command Acknowledge
  <UINT1 n ACK>
>

<S5F1 S Alarm Reports Send
  <LIST 6 
    <UINT2 n EQPID>
    <UINT2 n ALST>
    <UINT2 n ALCD>
    <UINT2 n ALID>
    <UINT2 n ALLVL>
    <ASCII n ALTX>
  >
>

<S5F2 P Alarm Report Acknowledge
  <BINARY 1 ACKC5>
>

<S5F101 S MES Alarm Report Send
  <LIST 3 
    <UINT2 n EQPID>
    <ASCII 3 EQPTYPE>
    <UINT2 n MRCD>
  >
>

<S5F102 P MES Alarm Report Acknowledge
  <BINARY 1 ACK5>
>

<S6F1 S Trace Data Send
  <LIST 3 
    <UINT1 n TRID>
    <UINT2 n LCOUNT>
    <UINT2 n UCOUNT>
  >
>

<S6F2 P Trace Data Acknowledge
  <BINARY 1 ACK5>
>

<S6F11 S Event Report Send
  <LIST 6 
    <UINT1 n CEID>
    <UINT2 n APPORT>
    <UINT1 n WORKTYPE>
    <UINT1 n ARSTATE>
    <LIST 2 
      <ASCII 10 TRAYID>
      <ASCII 4 ERCD>
    >
    <LIST 2 
      <ASCII 10 TRAYID>
      <ASCII 4 ERCD>
    >
  >
>

<S6F12 P Event Report Acknowledge
  <LIST 4 
    <UINT1 n ACK>
    <UINT1 n WORKTYPE>
    <UINT2 n APPORT>
    <LIST 2 
      <LIST 2 
        <ASCII 10 TRAYID>
        <UINT2 n MRCD>
      >
      <LIST 2 
        <ASCII 10 TRAYID>
        <UINT2 n MRCD>
      >
    >
  >
>

