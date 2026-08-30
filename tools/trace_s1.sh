#!/bin/bash
# 시나리오1 단독 추적 : 작업상태 · CV(121/122) · SC · RGV · HOST 전문 변화만 찍는다
Q() { sqlcmd -S 'localhost\SQLEXPRESS' -d LGLS_MCS_IO -E -W -h -1 -Q "SET NOCOUNT ON; $1" 2>/dev/null | grep -v '^$' | tr -d '\r'; }
prev=""
for i in $(seq 1 200); do
  job=$(Q "SELECT lugg_no+' st'+job_status+' '+start_pos+'>'+dest_pos FROM job_mst" | tr '\n' ';')
  cv=$(Q "SELECT mc_no+':lugg='+ISNULL(lugg_no_rd,'-')+' sen='+ISNULL(sensor0_data_rd,'-')+' dir='+ISNULL(stock_mode,'-')+' sto='+ISNULL(sto_ready_rd,'-')+' ret='+ISNULL(ret_ready_rd,'-')+' stoHS='+ISNULL(stohs_ready_rd,'-')+' rgvDep='+ISNULL(rtv_deparths_ready_rd,'-')+' rgvArr='+ISNULL(rtv_arrivehs_ready_rd,'-') FROM cv_data WHERE mc_no IN ('121','122','119','120') ORDER BY mc_no" | tr '\n' ';')
  rgv=$(Q "SELECT 'RGV req='+ISNULL(transfer_request_od,'-')+' lugg='+ISNULL(pallet_on_vehicle_rd,'-') FROM RTV_DATA_LGLS")
  sc=$(Q "SELECT 'SC'+sc_no+' req='+ISNULL(transfer_request_od,'-')+' pov='+ISNULL(pallet_on_vehicle_rd,'-') FROM SC_DATA_LGLS WHERE transfer_request_od='Y' OR ISNULL(pallet_on_vehicle_rd,'') NOT IN ('','0','0000')" | tr '\n' ';')
  cur="$job | $cv | $rgv | $sc"
  if [ "$cur" != "$prev" ]; then echo "$(date +%H:%M:%S) $cur"; prev="$cur"; fi
  sleep 2
done
