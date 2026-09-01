<XCom 2.4>
<S1F1 P Are You There
>

<S1F2 S On Line Data 
  <LIST 2 
    <ASCII 4 MDLN>
    <ASCII 6 SOFTREV>
  >
>

<S1F5 P Formatted Status Request
  <BINARY 1 SFCD>
>

<S1F6 S Formatted Status Data
  <LIST 2 
    <BINARY 1 SFCD>
    <LIST n 
      <LIST 5 
        <ASCII 10 EQPID>
        <UINT2 n TYPE>
        <UINT2 n STATE>
        <ASCII 4 ERCD>
        <UINT2 n ERTXT>
      >
    >
  >
>

<S1F13 P Establish Communications Request
  <LIST 2 
    <ASCII 4 MDLN>
    <ASCII 6 SOFTREV>
  >
>

<S1F13 S Establish Communications Request
  <LIST 0 >
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

<S1F14 S Establish Communications Request Acknowledge
  <LIST 2 
    <BINARY 1 COMMACK>
    <LIST 0 >
  >
>

<S1F101 P Equipment Status Send
  <LIST n 
    <LIST 2 
      <ASCII 10 EQPID>
      <UINT2 1 STATE>
    >
  >
>

<S1F102 S Equipment Status Send Acknowledge
  <BINARY 1 ACK6>
>

<S1F103 P Selected TRAY Status Request
  <LIST n 
    <UINT2 n TrackID>
  >
>

<S1F104 S Selected TRAY Status Data
  <LIST n 
    <LIST 3 
      <ASCII 6 TRAY ID>
      <ASCII 6 TRAY ID>
      <UINT2 n TRACK_STATUS>
    >
  >
>

<S2F13 P Equipment Constant Request
  <LIST n 
    <ASCII 4 ECID>
  >
>

<S2F14 S Equipment Constant Data
  <LIST n 
    <LIST 2 
      <ASCII 4 ECID>
      <ASCII 20 ECV>
    >
  >
>

<S2F15 P New Equipment Constant Send
  <LIST n 
    <LIST 2 
      <ASCII 4 ECID>
      <ASCII 20 ECV>
    >
  >
>

<S2F16 S New Equipment Constant Acknowledge
  <BINARY 1 ECACK>
>

<S2F17 P Date and Time Request
>

<S2F18 S Data and Time Data
  <ASCII 14 TIME>
>

<S2F31 P Data and Time Set Request
  <ASCII 14 TIME>
>

<S2F32 S Data and Time Set Acknowledge
  <BINARY 1 TIACK>
>

<S2F41 P Host Command Send (RCMD = 100 TRAY 이동)
  <LIST 6 
    <UINT2 n RCMD>
    <UINT2 n WORKTYPE>
    <UINT2 n LINE>
    <ASCII 4 FROM TRACK>
    <ASCII 4 TO TRACK>
    <LIST 2 
      <LIST 7 
        <ASCII 6 TRAY ID>
        <UINT2 n TOP_BOTTOM>
        <UINT2 n TRAY_TYPE>
        <UINT2 n TRAY_SIZE>
        <ASCII 1 ROTATE>
        <UINT2 n EMG_JOB>
        <ASCII 4 TRACK>
      >
      <LIST 7 
        <ASCII 6 TRAY ID>
        <UINT2 n TOP_BOTTOM>
        <UINT2 n TRAY_TYPE>
        <UINT2 n TRAY_SIZE>
        <ASCII 1 ROTATE>
        <UINT2 n EMG_JOB>
        <ASCII 4 TRACK>
      >
    >
  >
>

<S2F41 P Host Command Send (RCMD = 100 TRAY 이동)
  <LIST 6 
    <UINT2 n RCMD>
    <UINT2 n WORKTYPE>
    <UINT2 n LINE>
    <ASCII 4 FROM TRACK>
    <ASCII 4 TO TRACK>
    <LIST 2 
      <LIST 7 
        <ASCII 6 TRAY ID>
        <UINT2 n TOP_BOTTOM>
        <UINT2 n TRAY_TYPE>
        <UINT2 n TRAY_SIZE>
        <ASCII 1 ROTATE>
        <UINT2 n EMG_JOB>
        <ASCII 4 TRACK>
      >
      <LIST 0 >
    >
  >
>

<S2F41 P Host Command Send (RCMD = 101-TRAY 입고) 
  <LIST 7 
    <UINT2 n RCMD = 101>
    <UINT2 n WORKTYPE = 1>
    <ASCII 5 STACKER_ID>
    <UINT2 n IN_OUT_TYPE(Single, Twin, Double)>
    <UINT2 n TIME_SPEC>
    <LIST 5 
      <ASCII 4 TRACK_ID (1st 입고 Track) >
      <ASCII 6 TRAY_ID (Top Tray ID)>
      <ASCII 6 TRAY_ID (Bottom Tray ID)>
      <ASCII 4 STAGE_NO(Only 충방전기) >
      <ASCII 10 TO_LOCATION>
    >
    <LIST 5 
      <ASCII 4 TRACK_ID (2nd 입고 Track) >
      <ASCII 6 TRAY_ID (Top Tray ID)>
      <ASCII 6 TRAY_ID (Bottom Tray ID)>
      <ASCII 4 STAGE_NO(Only 충방전기) >
      <ASCII 10 TO_LOCATION>
    >
  >
>

<S2F41 P Host Command Send (RCMD = 101-TRAY 입고) 
  <LIST 7 
    <UINT2 n RCMD = 101>
    <UINT2 n WORKTYPE = 1>
    <ASCII 5 STACKER_ID>
    <UINT2 n IN_OUT_TYPE(Single, Twin, Double)>
    <UINT2 n TIME_SPEC>
    <LIST 5 
      <ASCII 4 TRACK_ID (1st 입고 Track) >
      <ASCII 6 TRAY_ID (Top Tray ID)>
      <ASCII 6 TRAY_ID (Bottom Tray ID)>
      <ASCII 4 STAGE_NO(Only 충방전기) >
      <ASCII 10 TO_LOCATION>
    >
    <LIST 0 >
  >
>

<S2F41 P Host Command Send (RCMD = 102-TRAY 출고) 
  <LIST 7 
    <UINT2 n RCMD = 102>
    <UINT2 n WORKTYPE = 2>
    <ASCII 5 STACKER_ID>
    <UINT2 n IN_OUT_TYPE(Single, Twin, Double)>
    <UINT2 n TIME_SPEC>
    <LIST 5 
      <ASCII 4 TRACK_ID (1st 입고 Track) >
      <ASCII 6 TRAY_ID (Top Tray ID)>
      <ASCII 6 TRAY_ID (Bottom Tray ID)>
      <ASCII 4 STAGE_NO(Only 충방전기) >
      <ASCII 10 TO_LOCATION>
    >
    <LIST 5 
      <ASCII 4 TRACK_ID (2nd 입고 Track) >
      <ASCII 6 TRAY_ID (Top Tray ID)>
      <ASCII 6 TRAY_ID (Bottom Tray ID)>
      <ASCII 4 STAGE_NO(Only 충방전기) >
      <ASCII 10 TO_LOCATION>
    >
  >
>

<S2F41 P Host Command Send (RCMD = 102-TRAY 출고) 
  <LIST 7 
    <UINT2 n RCMD = 102>
    <UINT2 n WORKTYPE = 2>
    <ASCII 5 STACKER_ID>
    <UINT2 n IN_OUT_TYPE(Single, Twin, Double)>
    <UINT2 n TIME_SPEC>
    <LIST 5 
      <ASCII 4 TRACK_ID (1st 입고 Track) >
      <ASCII 6 TRAY_ID (Top Tray ID)>
      <ASCII 6 TRAY_ID (Bottom Tray ID)>
      <ASCII 4 STAGE_NO(Only 충방전기) >
      <ASCII 10 TO_LOCATION>
    >
    <LIST 0 >
  >
>

<S2F41 P Host Command Send (RCMD = 103-TRAY 2단 적재  대기 명령 ) 
  <LIST 6 
    <UINT2 n RCMD = 103>
    <UINT2 n WORKTYPE = 3>
    <UINT2 n TRAY TYPE>
    <ASCII 6 TRAY_ID >
    <ASCII 4 TRACK_ID >
    <UINT2 n TIME_SPEC>
  >
>

<S2F41 P Host Command Send (RCMD = 104-랙투랙)  
  <LIST 7 
    <UINT2 n RCMD = 104>
    <UINT2 n WORKTYPE = 4>
    <ASCII 5 STACKER_ID>
    <ASCII 6 TRAY_ID (Top Tray ID)>
    <ASCII 6 TRAY_ID (Bottom Tray ID)>
    <LIST 2 
      <ASCII 4 STAGE_NO>
      <ASCII 10 TO_LOCATION>
    >
    <LIST 2 
      <ASCII 4 STAGE_NO>
      <ASCII 10 TO_LOCATION>
    >
  >
>

<S2F41 P Host Command Send (RCMD = 105 TRAY 분리 지시)
  <LIST 5 
    <UINT2 n RCMD = 105>
    <UINT2 n WORK_TYPE >
    <ASCII 4 TRACK_NO >
    <ASCII 6 TRAY_ID >
    <ASCII 6 TRAY_ID >
  >
>

<S2F41 P Host Command Send (RCMD = 200-화재 발생 강제 이동 명령)  
  <LIST 4 
    <UINT2 n RCMD = 200>
    <UINT2 n WORKTYPE = 7>
    <UINT2 n ERROR_CODE(22:SMOKE ERROR, 23:TEMP ALARM)>
    <LIST n 
      <ASCII 10 TO_LOCATION>
    >
  >
>

<S2F42 S Host Command Acknowledge
  <BINARY 1 HCACK>
>

<S5F1 P Alarm Reports Send
  <LIST 6 
    <ASCII 10 EQPID>
    <UINT2 n ALST>
    <UINT2 n ALCD>
    <ASCII 4 ALID>
    <UINT2 n ALLVL>
    <ASCII 200 ALTX>
  >
>

<S5F2 S Alarm Report Acknowledge
  <BINARY 1 ACKC5>
>

<S5F101 P MES Alarm Report Send
  <LIST 2 
    <UINT2 n MID>
    <UINT2 n MRCD>
  >
>

<S5F102 S MES Alarm Report Acknowledge
  <BINARY 1 ACK5>
>

<S6F1 P Event Report Send (버퍼 대기수량)
  <LIST 4 
    <ASCII 4 TRACK_NO>
    <UINT2 n BUFFER_SIZE>
    <UINT2 n EXIST_TRACK_CNT>
    <UINT2 n EMPTY_TRACK_CNT>
  >
>

<S6F2 S Event Report Acknowledge (버퍼 대기수량)
  <BINARY 1 ACKC6>
>

<S6F11 P Event Report Send (CEID = 101 TRAY 도착 보고)
  <LIST 3 
    <UINT2 n DATA ID = 1>
    <UINT2 n CEID = 101>
    <LIST 2 
      <LIST 2 
        <UINT2 n RPTID=100 (Track 정보)>
        <LIST 4 
          <ASCII 4 FROM_TRACK>
          <ASCII 4 TO_TRACK>
          <UINT2 n WORK_TYPE>
          <UINT2 n TO_TRACK_STATUS>
        >
      >
      <LIST 2 
        <UINT2 n RPTID=101 (Tray 정보)>
        <LIST 2 
          <LIST 2 
            <ASCII 6 TRAYID>
            <ASCII 4 ALID>
          >
          <LIST 2 
            <ASCII 6 TRAYID>
            <ASCII 4 ALID>
          >
        >
      >
    >
  >
>

<S6F11 P Event Report Send (CEID = 102 TRAY 출발 보고)
  <LIST 3 
    <UINT2 n DATA ID = 1>
    <UINT2 n CEID = 102>
    <LIST 2 
      <LIST 2 
        <UINT2 n RPTID=100 (Track 정보)>
        <LIST 4 
          <ASCII 4 FROM_TRACK>
          <ASCII 4 TO_TRACK>
          <UINT2 n WORK_TYPE>
          <UINT2 n TO_TRACK_STATUS = 0>
        >
      >
      <LIST 2 
        <UINT2 n RPTID=101 (Tray 정보)>
        <LIST 2 
          <LIST 2 
            <ASCII 6 TRAYID>
            <ASCII 4 ALID>
          >
          <LIST 2 
            <ASCII 6 TRAYID>
            <ASCII 4 ALID>
          >
        >
      >
    >
  >
>

<S6F11 P Event Report Send (CEID = 103 수동 TRAY 도착 보고)
  <LIST 3 
    <UINT2 n DATA ID = 1>
    <UINT2 n CEID = 103>
    <LIST 2 
      <LIST 2 
        <UINT2 n RPTID=100 (Track 정보)>
        <LIST 4 
          <ASCII 4 FROM_TRACK>
          <ASCII 4 TO_TRACK>
          <UINT2 n WORK_TYPE>
          <UINT2 n TO_TRACK_STATUS>
        >
      >
      <LIST 2 
        <UINT2 n RPTID=101 (Tray 정보)>
        <LIST 2 
          <LIST 2 
            <ASCII 6 TRAYID>
            <ASCII 4 ALID>
          >
          <LIST 2 
            <ASCII 6 TRAYID>
            <ASCII 4 ALID>
          >
        >
      >
    >
  >
>

<S6F11 P Event Report Send (CEID = 201 Tray 입고 시작 보고, CEID = 202 Tray 입고 완료 보고)
  <LIST 3 
    <UINT2 n DATA ID = 1>
    <UINT2 n CEID>
    <LIST 2 
      <LIST 2 
        <UINT2 n RPTID=200 (Track 정보)>
        <LIST 5 
          <ASCII 4 TRACKID (1st 입고 Track ID) >
          <ASCII 4 TRACKID (2nd 입고 Track ID)>
          <UINT2 n IN_OUT_TYPE>
          <UINT2 n WORK_TYPE>
          <ASCII 5 STACKER_ID >
        >
      >
      <LIST 2 
        <UINT2 n RPTID=201 (Tray 정보 및 입고 Location 정보)>
        <LIST 2 
          <LIST 5 Front Fork Info
            <ASCII 6 TRAYID(하단 Tray ID)>
            <ASCII 6 TRAYID(상단 Tray ID)>
            <ASCII 4 STAGE_NO (Only 충방전기)>
            <ASCII 10 LOCATION (입고 Location)>
            <ASCII 4 ALARM_ID>
          >
          <LIST 5 Rear Fork Info
            <ASCII 6 TRAYID(하단 Tray ID)>
            <ASCII 6 TRAYID(상단 Tray ID)>
            <ASCII 4 STAGE_NO (Only 충방전기)>
            <ASCII 10 LOCATION (입고 Location)>
            <ASCII 4 ALARM_ID>
          >
        >
      >
    >
  >
>

<S6F11 P Event Report Send (CEID = 203 Tray 출고 시작 보고, CEID = 204 Tray 출고 완료 보고)
  <LIST 3 
    <UINT2 n DATA ID = 1>
    <UINT2 n CEID>
    <LIST 2 
      <LIST 2 
        <UINT2 n RPTID=200 (Track 정보)>
        <LIST 5 
          <ASCII 4 TRACKID (1st 입고 Track ID)>
          <ASCII 4 TRACKID (2nd 입고 Track ID)>
          <UINT2 n IN_OUT_TYPE>
          <UINT2 n WORK_TYPE>
          <ASCII 5 STACKER_ID>
        >
      >
      <LIST 2 
        <UINT2 n RPTID=201 (Tray 정보 및 출고 Location 정보)>
        <LIST 2 
          <LIST 5 Front Fork Info
            <ASCII 6 TRAYID(하단 Tray ID)>
            <ASCII 6 TRAYID(상단 Tray ID)>
            <ASCII 4 STAGE_NO (Only 충방전기)>
            <ASCII 10 LOCATION (출고 Location)>
            <ASCII 4 ALARM_ID>
          >
          <LIST 5 Rear Fork Info
            <ASCII 6 TRAYID(하단 Tray ID)>
            <ASCII 6 TRAYID(상단 Tray ID)>
            <ASCII 4 STAGE_NO (Only 충방전기)>
            <ASCII 10 LOCATION (출고 Location)>
            <ASCII 4 ALARM_ID>
          >
        >
      >
    >
  >
>

<S6F11 P Event Report Send (CEID = 205 Tray 이동 시작 보고, CEID = 206 Tray 이동 완료 보고)
  <LIST 3 
    <UINT2 n DATA ID = 1>
    <UINT2 n CEID>
    <LIST 2 
      <LIST 2 
        <UINT2 n RPTID=205 (Stacker & Track 정보)>
        <LIST 5 
          <ASCII 5 STACKER_ID>
          <ASCII 6 TRAYID (Top TrayID)>
          <ASCII 6 TRAYID (Bottom TrayID)>
          <UINT2 n WORK_TYPE>
          <ASCII 4 ALID>
        >
      >
      <LIST 2 
        <UINT2 n RPTID=206 (Location 정보)>
        <LIST 4 
          <ASCII 4 STAGE_NO - FROM>
          <ASCII 10 LOCATION - FROM>
          <ASCII 4 STAGE_NO - TO>
          <ASCII 10 LOCATION - TO>
        >
      >
    >
  >
>

<S6F11 P Event Report Send (CEID = 207 분리 보고)
  <LIST 3 
    <UINT2 n DATA ID = 1>
    <UINT2 n CEID = 207>
    <LIST 1 
      <LIST 2 
        <UINT2 n RPTID=207 (RPTID)>
        <LIST 4 
          <ASCII 4 TRACK_ID>
          <ASCII 6 TRAY_ID>
          <UINT2 n TOP_BOTTOM>
          <ASCII 4 ALID>
        >
      >
    >
  >
>

<S6F11 P Event Report Send (CEID = 301 Stacker 보고)
  <LIST 3 
    <UINT2 n DATA ID = 1>
    <UINT2 n CEID = 301>
    <LIST 1 
      <LIST 2 
        <UINT2 n RPTID=220 (Stacker 정보)>
        <LIST 1 
          <LIST 9 
            <ASCII 5 STACKER_ID>
            <UINT2 n STACKER_STATUS>
            <UINT2 n ERROR_CODE>
            <ASCII 6 POS (Stacker Crane Position)>
            <UINT2 n FHSF (PHS Front)>
            <UINT2 n FHSR (PHS Rear)>
            <UINT2 n LOADF (Load Front_Exist)>
            <UINT2 n LOADF (Load Rear_Exist)>
            <UINT2 n OJC>
          >
        >
      >
    >
  >
>

<S6F11 P Event Report Send (CEID = 303 Stacker 운영 보고)
  <LIST 3 
    <UINT2 n DATA ID = 1>
    <UINT2 n CEID = 303>
    <LIST 2 
      <LIST 2 
        <UINT2 n RPTID=230 (Stacker 정보)>
        <LIST 9 
          <ASCII 5 STACKER_ID>
          <UINT2 n WORK_TYPE>
          <ASCII 14 CMD_TIME(MES 지시 시간)>
          <ASCII 14 SC_START_TIME(Stacker Crane 구동 시작 시간)>
          <ASCII 10 START_LOCATION (구동 시작 좌표)>
          <ASCII 10 LOCATION_F (전 Fork Location)>
          <ASCII 10 LOCATION_R (후 Fork Location)>
          <ASCII 14 WORK_COMPLETE_TIME(Stacker Crane 작업 완료 시간)>
          <ASCII 10 IN_OUT_POSITION (입출고대 좌표)>
        >
      >
      <LIST 2 
        <UINT2 n RPTID=231 (Tray 정보)>
        <LIST 2 
          <LIST 6 전 Fork Tray ID
            <ASCII 6 TRAYID (상단 TrayID)>
            <ASCII 6 TRAYID (하단 TrayID)>
            <ASCII 14 PICK_TIME(화물 적재 시간)>
            <ASCII 14 IN_HOME_POSTIME(화물 위치 도착 시간-입고 시 Home Position)>
            <ASCII 14 IN_HOME_POSTIME(화물 위치 도착 시간-출고 시 Home Position)>
            <ASCII 14 WORK_COMPLETE_TIME(Stacker Crane 작업 완료 시간)>
          >
          <LIST 6 후 Fork Tray ID
            <ASCII 6 TRAYID (상단 TrayID)>
            <ASCII 6 TRAYID (하단 TrayID)>
            <ASCII 14 PICK_TIME(화물 적재 시간)>
            <ASCII 14 IN_HOME_POSTIME(화물 위치 도착 시간-입고 시 Home Position)>
            <ASCII 14 IN_HOME_POSTIME(화물 위치 도착 시간-출고 시 Home Position)>
            <ASCII 14 WORK_COMPLETE_TIME(Stacker Crane 작업 완료 시간)>
          >
        >
      >
    >
  >
>

<S6F11 P Event Report Send (CEID = 401 Tray 입고 준비)
  <LIST 3 
    <UINT2 n DATA ID = 1>
    <UINT2 n CEID = 401>
    <LIST 1 
      <LIST 2 
        <UINT2 n RPTID=400 (Tray & Track 정보)>
        <LIST 2 
          <LIST 3 1st 입고대 
            <ASCII 4 TRACK_ID  (입고대 TrackID)>
            <ASCII 6 TRAYID (상단 Tray ID)>
            <ASCII 6 TRAYID (하단 Tray ID)>
          >
          <LIST 3 2nd 입고대 
            <ASCII 4 TRACK_ID  (입고대 TrackID)>
            <ASCII 6 TRAYID (상단 Tray ID)>
            <ASCII 6 TRAYID (하단 Tray ID)>
          >
        >
      >
    >
  >
>

<S6F11 P Event Report Send (CEID = 501 Equiment Constants Change)
  <LIST 3 
    <UINT2 n DATA ID = 1>
    <UINT2 n CEID = 501>
    <LIST 1 
      <LIST 2 
        <UINT2 n RPTID=500 (Equipment Constants 정보)>
        <LIST n 
          <LIST 2 
            <ASCII 5 TRACK_NO>
            <ASCII 20 SEC>
          >
        >
      >
    >
  >
>

<S6F12 S Event Report Acknowledge
  <LIST 4 
    <UINT2 1 ERACK6>
    <UINT2 1 CEID>
    <ASCII 4 TRACK_NO>
    <LIST n 
      <LIST 2 
        <ASCII 6 TRAY ID>
        <UINT2 n MRCD>
      >
    >
  >
>

