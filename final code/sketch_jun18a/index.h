const char HTML_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="fr">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Supervision Solaire — ENSPD</title>
<link rel="preconnect" href="https://fonts.googleapis.com">
<link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
<link href="https://fonts.googleapis.com/css2?family=Plus+Jakarta+Sans:wght@400;500;600;700;800&display=swap" rel="stylesheet">
<script src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/4.4.1/chart.umd.min.js"></script>
<style>
:root {
  --cream:#F5F1E8; --cream-card:#FBF8F1; --cream-deep:#EFE9DA;
  --yellow:#F4D03F; --yellow-deep:#E8C220; --ink:#1C1C1A; --ink-soft:#2A2A28;
  --grey:#8A8578; --grey-light:#B8B2A4; --green:#4CAF6D; --green-bg:#E3F3E8;
  --red:#E05B4A; --red-bg:#FBE7E3; --blue:#3B7DD8; --blue-bg:#E8F0FB;
  --line:#E5DECF; --radius:16px; --radius-sm:11px;
  --shadow:0 6px 22px rgba(120,110,80,0.10); --shadow-soft:0 3px 12px rgba(120,110,80,0.07);
}
* { margin:0; padding:0; box-sizing:border-box; }
body { font-family:'Plus Jakarta Sans',sans-serif; background:linear-gradient(160deg,#F5F1E8 0%,#F0EADb 50%,#F5EFDF 100%); color:var(--ink); min-height:100vh; -webkit-font-smoothing:antialiased; padding-bottom:90px; }
svg.icon { width:100%; height:100%; display:block; stroke-width:1.9; }
.topbar { display:flex; align-items:center; justify-content:space-between; padding:18px 28px; max-width:1280px; margin:0 auto; }
.brand { display:flex; align-items:center; gap:11px; font-size:1.25rem; font-weight:800; letter-spacing:-0.5px; }
.brand-logo { width:36px; height:36px; border-radius:11px; background:var(--yellow); display:grid; place-items:center; box-shadow:0 3px 8px rgba(232,194,32,0.4); }
.brand-logo svg { width:21px; height:21px; stroke:var(--ink); }
.nav-pc { display:flex; gap:4px; background:var(--cream-card); padding:5px; border-radius:30px; box-shadow:var(--shadow-soft); }
.nav-pc button { border:none; background:transparent; padding:9px 22px; border-radius:22px; font-size:0.9rem; font-weight:600; color:var(--grey); cursor:pointer; transition:all 0.2s; font-family:inherit; }
.nav-pc button.active { background:var(--ink); color:white; box-shadow:0 3px 10px rgba(28,28,26,0.25); }
.topbar-right { display:flex; align-items:center; gap:12px; }
.status-pill { display:flex; align-items:center; gap:8px; background:var(--cream-card); padding:8px 16px; border-radius:22px; font-size:0.8rem; font-weight:600; box-shadow:var(--shadow-soft); }
.dot { width:9px; height:9px; border-radius:50%; background:var(--green); box-shadow:0 0 0 3px var(--green-bg); animation:pulse 2s infinite; }
.dot.off { background:var(--red); box-shadow:0 0 0 3px var(--red-bg); }
@keyframes pulse { 50% { opacity:0.5; } }
.container { max-width:1280px; margin:0 auto; padding:8px 24px 40px; }
.page-title { font-size:2rem; font-weight:800; letter-spacing:-1px; margin:14px 0 4px; }
.page-sub { color:var(--grey); font-size:0.9rem; margin-bottom:24px; font-weight:500; }
.tab-content { display:none; animation:fade 0.35s ease; }
.tab-content.active { display:block; }
@keyframes fade { from { opacity:0; transform:translateY(8px); } to { opacity:1; transform:none; } }
.grid { display:grid; gap:18px; }
.grid-dash { grid-template-columns:1.1fr 1fr 1fr; grid-template-areas:"soc stats stats" "soc relays relays"; }
.card { background:var(--cream-card); border-radius:var(--radius); padding:22px; box-shadow:var(--shadow); border:1px solid rgba(255,255,255,0.6); }
.card-dark { background:var(--ink); color:var(--cream); }
.card-head { display:flex; align-items:center; justify-content:space-between; margin-bottom:16px; }
.card-head h3 { font-size:1rem; font-weight:700; }
.card-head .ico { width:32px; height:32px; border-radius:9px; background:var(--cream-deep); display:grid; place-items:center; }
.card-head .ico svg { width:17px; height:17px; stroke:var(--ink); }
.soc-card { grid-area:soc; display:flex; flex-direction:column; }
.ring-wrap { flex:1; display:flex; flex-direction:column; align-items:center; justify-content:center; gap:4px; padding:10px 0; }
.ring { position:relative; width:230px; height:230px; }
.ring svg.gauge { transform:rotate(-90deg); }
.ring-bg { fill:none; stroke:var(--cream-deep); stroke-width:18; }
.ring-fill { fill:none; stroke:var(--yellow); stroke-width:18; stroke-linecap:round; transition:stroke-dashoffset 1s ease,stroke 0.5s; }
.ring-center { position:absolute; inset:0; display:flex; flex-direction:column; align-items:center; justify-content:center; }
.ring-val { font-size:3.4rem; font-weight:800; letter-spacing:-2px; line-height:1; }
.ring-label { font-size:0.78rem; color:var(--grey); font-weight:600; margin-top:4px; letter-spacing:1px; }
.ring-sub { margin-top:8px; padding:5px 14px; background:var(--green-bg); color:var(--green); border-radius:16px; font-size:0.78rem; font-weight:700; }
.ring-sub.warn { background:#FDF3DD; color:var(--yellow-deep); }
.ring-sub.crit { background:var(--red-bg); color:var(--red); }
.autonomy { display:flex; justify-content:space-around; border-top:1px solid var(--line); padding-top:16px; margin-top:6px; }
.autonomy div { text-align:center; }
.autonomy .big { font-size:1.3rem; font-weight:800; }
.autonomy .lbl { font-size:0.72rem; color:var(--grey); font-weight:500; }
.stats-card { grid-area:stats; }
.stats-row { display:grid; grid-template-columns:repeat(3,1fr); gap:14px; }
.stat { background:var(--cream); border-radius:var(--radius-sm); padding:18px 16px; text-align:center; }
.stat .num { font-size:2rem; font-weight:800; letter-spacing:-1px; }
.stat .unit { font-size:0.95rem; color:var(--grey); font-weight:600; }
.stat .lbl { font-size:0.72rem; color:var(--grey); font-weight:600; text-transform:uppercase; letter-spacing:0.5px; margin-top:4px; }
.stat .ico-top { width:26px; height:26px; margin:0 auto 8px; }
.stat .ico-top svg { stroke:var(--yellow-deep); }
.relays-card { grid-area:relays; }
.relay { display:flex; align-items:center; gap:14px; padding:14px 0; border-bottom:1px solid var(--line); }
.relay:last-child { border-bottom:none; }
.relay-ico { width:44px; height:44px; border-radius:12px; background:var(--cream); display:grid; place-items:center; flex-shrink:0; }
.relay-ico svg { width:21px; height:21px; stroke:var(--ink); }
.relay-info { flex:1; }
.relay-name { font-weight:700; font-size:0.95rem; }
.relay-state { font-size:0.78rem; color:var(--grey); margin-top:2px; font-weight:500; }
.relay-mode { font-size:0.68rem; font-weight:700; padding:4px 10px; border-radius:10px; background:var(--cream-deep); color:var(--grey); cursor:pointer; user-select:none; letter-spacing:0.3px; }
.relay-mode.auto { background:var(--blue-bg); color:var(--blue); }
.switch { position:relative; width:52px; height:30px; flex-shrink:0; cursor:pointer; }
.switch input { opacity:0; width:0; height:0; }
.slider { position:absolute; inset:0; border-radius:30px; background:var(--grey-light); transition:0.3s; }
.slider:before { content:""; position:absolute; width:24px; height:24px; left:3px; top:3px; background:white; border-radius:50%; transition:0.3s; box-shadow:0 2px 5px rgba(0,0,0,0.2); }
.switch input:checked + .slider { background:var(--green); }
.switch input:checked + .slider:before { transform:translateX(22px); }
.switch input:disabled + .slider { opacity:0.5; cursor:not-allowed; }
.grid-analyse { grid-template-columns:1fr 1fr; grid-template-areas:"chart1 chart1" "chart2 kwh"; }
.chart-soc { grid-area:chart1; }
.chart-pow { grid-area:chart2; }
.kwh-card { grid-area:kwh; display:flex; flex-direction:column; justify-content:center; }
.kwh-big { font-size:3rem; font-weight:800; letter-spacing:-2px; }
.kwh-unit { font-size:1.1rem; color:var(--grey-light); }
.chart-box { position:relative; height:260px; }
.chart-box-sm { position:relative; height:200px; }
.grid-journal { grid-template-columns:1fr 1fr; grid-template-areas:"logs logs" "downtime maint"; }
.logs-card { grid-area:logs; }
.downtime-card { grid-area:downtime; }
.maint-card { grid-area:maint; }
.log-table { width:100%; border-collapse:collapse; }
.log-scroll { max-height:320px; overflow-y:auto; }
.log-table th { text-align:left; font-size:0.72rem; color:var(--grey); text-transform:uppercase; letter-spacing:0.5px; padding:8px 10px; position:sticky; top:0; background:var(--cream-card); font-weight:600; }
.log-table td { padding:11px 10px; font-size:0.85rem; border-top:1px solid var(--line); font-weight:500; }
.log-time { color:var(--grey); font-size:0.78rem; white-space:nowrap; }
.log-badge { font-size:0.7rem; font-weight:700; padding:3px 10px; border-radius:10px; }
.b-info { background:var(--blue-bg); color:var(--blue); }
.b-warn { background:#FDF3DD; color:var(--yellow-deep); }
.b-crit { background:var(--red-bg); color:var(--red); }
.b-ok { background:var(--green-bg); color:var(--green); }
.downtime-big { font-size:2.6rem; font-weight:800; letter-spacing:-1.5px; }
.btn { border:none; padding:11px 18px; border-radius:12px; font-weight:700; font-size:0.82rem; cursor:pointer; font-family:inherit; transition:all 0.2s; display:inline-flex; align-items:center; gap:7px; }
.btn svg { width:15px; height:15px; }
.btn-light { background:var(--cream-deep); color:var(--ink); }
.btn-light svg { stroke:var(--ink); }
.maint-toggle { display:flex; align-items:center; justify-content:space-between; padding:14px 0; border-top:1px solid var(--line); margin-top:14px; }
.nav-mobile { display:none; position:fixed; bottom:0; left:0; right:0; background:var(--cream-card); border-top:1px solid var(--line); padding:10px 8px calc(10px + env(safe-area-inset-bottom)); justify-content:space-around; box-shadow:0 -4px 20px rgba(120,110,80,0.08); z-index:100; }
.nav-mobile button { border:none; background:transparent; cursor:pointer; display:flex; flex-direction:column; align-items:center; gap:4px; font-size:0.68rem; font-weight:600; color:var(--grey); font-family:inherit; padding:5px 18px; border-radius:12px; transition:all 0.2s; }
.nav-mobile button .nav-ico { width:22px; height:22px; }
.nav-mobile button .nav-ico svg { stroke:var(--grey); }
.nav-mobile button.active { color:var(--ink); background:var(--cream-deep); }
.nav-mobile button.active .nav-ico svg { stroke:var(--ink); }
@media (max-width:880px) {
  .nav-pc { display:none; }
  .nav-mobile { display:flex; }
  .topbar { padding:16px 20px; }
  .container { padding:4px 16px 30px; }
  .page-title { font-size:1.5rem; }
  .grid-dash,.grid-analyse,.grid-journal { grid-template-columns:1fr; grid-template-areas:none; }
  .grid-dash>*,.grid-analyse>*,.grid-journal>* { grid-area:auto !important; }
  .ring { width:200px; height:200px; }
  .ring-val { font-size:2.8rem; }
  .stats-row { grid-template-columns:1fr; }
  .stat { display:flex; align-items:center; gap:14px; text-align:left; }
  .stat .ico-top { margin:0; }
  .stat-body { flex:1; }
}
</style>
</head>
<body>
<div class="topbar">
  <div class="brand">
    <div class="brand-logo"><svg class="icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="4"/><path d="M12 2v2M12 20v2M4.9 4.9l1.4 1.4M17.7 17.7l1.4 1.4M2 12h2M20 12h2M6.3 17.7l-1.4 1.4M19.1 4.9l-1.4 1.4"/></svg></div>
    <span>SolarSup</span>
  </div>
  <nav class="nav-pc">
    <button class="active" data-tab="dash">Dashboard</button>
    <button data-tab="analyse">Analyse</button>
    <button data-tab="journal">Journal</button>
  </nav>
  <div class="topbar-right">
    <div class="status-pill"><span class="dot" id="conn-dot"></span><span id="conn-text">En ligne</span></div>
  </div>
</div>
<div class="container">
  <h1 class="page-title">Supervision Solaire</h1>
  <p class="page-sub">Installation PV — Bloc administratif ENSPD</p>
  <div class="tab-content active" id="dash">
    <div class="grid grid-dash">
      <div class="card soc-card">
        <div class="card-head"><h3>État de charge</h3><div class="ico"><svg class="icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-linecap="round" stroke-linejoin="round"><rect x="2" y="7" width="16" height="10" rx="2"/><path d="M22 11v2"/><path d="M6 11v2"/></svg></div></div>
        <div class="ring-wrap"><div class="ring">
          <svg class="gauge" width="230" height="230"><circle class="ring-bg" cx="115" cy="115" r="100"></circle><circle class="ring-fill" id="soc-ring" cx="115" cy="115" r="100" stroke-dasharray="628" stroke-dashoffset="628"></circle></svg>
          <div class="ring-center"><div class="ring-val" id="soc-val">--</div><div class="ring-label">BATTERIE</div><div class="ring-sub" id="soc-state">En attente</div></div>
        </div></div>
        <div class="autonomy"><div><div class="big" id="autonomy">--</div><div class="lbl">Autonomie estimée</div></div><div><div class="big" id="vbat-mini">-- V</div><div class="lbl">Tension batterie</div></div></div>
      </div>
      <div class="card stats-card">
        <div class="card-head"><h3>Mesures temps réel</h3><div class="ico"><svg class="icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-linecap="round" stroke-linejoin="round"><path d="M13 2 3 14h9l-1 8 10-12h-9z"/></svg></div></div>
        <div class="stats-row">
          <div class="stat"><div class="ico-top"><svg class="icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-linecap="round" stroke-linejoin="round"><path d="M7 7h10v6a5 5 0 0 1-10 0z"/><path d="M9 3v4M15 3v4M12 18v3"/></svg></div><div class="stat-body"><span class="num" id="stat-v">--</span><span class="unit"> V</span><div class="lbl">Tension</div></div></div>
          <div class="stat"><div class="ico-top"><svg class="icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-linecap="round" stroke-linejoin="round"><path d="M3 3v18h18"/><path d="m7 14 3-3 3 3 4-5"/></svg></div><div class="stat-body"><span class="num" id="stat-a">--</span><span class="unit"> A</span><div class="lbl">Courant</div></div></div>
          <div class="stat"><div class="ico-top"><svg class="icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-linecap="round" stroke-linejoin="round"><path d="M9 18h6M10 22h4M12 2a7 7 0 0 0-4 12.7c.6.5 1 1.3 1 2.3h6c0-1 .4-1.8 1-2.3A7 7 0 0 0 12 2z"/></svg></div><div class="stat-body"><span class="num" id="stat-w">--</span><span class="unit"> W</span><div class="lbl">Puissance</div></div></div>
        </div>
        <div style="margin-top:16px; padding-top:16px; border-top:1px solid var(--line); display:flex; justify-content:space-between; align-items:center;">
          <div><div style="font-size:0.72rem; color:var(--grey); font-weight:600; text-transform:uppercase; letter-spacing:0.5px;">Tension onduleur (AC)</div><div style="font-size:1.6rem; font-weight:800; margin-top:2px;"><span id="stat-vac">--</span> V</div></div>
          <div style="font-size:0.76rem; color:var(--grey); text-align:right; font-weight:500;">Sortie 220V<br>onduleur</div>
        </div>
      </div>
      <div class="card relays-card">
        <div class="card-head"><h3>Contrôle des circuits</h3><div class="ico"><svg class="icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-linecap="round" stroke-linejoin="round"><line x1="4" y1="21" x2="4" y2="14"/><line x1="4" y1="10" x2="4" y2="3"/><line x1="12" y1="21" x2="12" y2="12"/><line x1="12" y1="8" x2="12" y2="3"/><line x1="20" y1="21" x2="20" y2="16"/><line x1="20" y1="12" x2="20" y2="3"/><line x1="1" y1="14" x2="7" y2="14"/><line x1="9" y1="8" x2="15" y2="8"/><line x1="17" y1="16" x2="23" y2="16"/></svg></div></div>
        <div class="relay"><div class="relay-ico"><svg class="icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-linecap="round" stroke-linejoin="round"><path d="M9 18h6M10 22h4M12 2a7 7 0 0 0-4 12.7c.6.5 1 1.3 1 2.3h6c0-1 .4-1.8 1-2.3A7 7 0 0 0 12 2z"/></svg></div><div class="relay-info"><div class="relay-name">Éclairage</div><div class="relay-state" id="r1-state">—</div></div><span class="relay-mode auto" id="r1-mode" onclick="toggleMode('r1')">AUTO</span><label class="switch"><input type="checkbox" id="r1-sw" onchange="cmdRelay('R1', this.checked)"><span class="slider"></span></label></div>
        <div class="relay"><div class="relay-ico"><svg class="icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-linecap="round" stroke-linejoin="round"><rect x="4" y="4" width="16" height="16" rx="2"/><path d="M9 9v2M15 9v2M9 15h6"/></svg></div><div class="relay-info"><div class="relay-name">Prises / PC</div><div class="relay-state" id="r2-state">—</div></div><span class="relay-mode auto" id="r2-mode" onclick="toggleMode('r2')">AUTO</span><label class="switch"><input type="checkbox" id="r2-sw" onchange="cmdRelay('R2', this.checked)"><span class="slider"></span></label></div>
        <div class="relay"><div class="relay-ico"><svg class="icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-linecap="round" stroke-linejoin="round"><path d="M12 2v20M2 12h20M5 5l14 14M19 5 5 19"/></svg></div><div class="relay-info"><div class="relay-name">Climatisation</div><div class="relay-state" id="r3-state">—</div></div><span class="relay-mode auto" id="r3-mode" onclick="toggleMode('r3')">AUTO</span><label class="switch"><input type="checkbox" id="r3-sw" onchange="cmdRelay('R3', this.checked)"><span class="slider"></span></label></div>
      </div>
    </div>
  </div>
  <div class="tab-content" id="analyse">
    <div class="grid grid-analyse">
      <div class="card chart-soc"><div class="card-head"><h3>Historique état de charge — 24h</h3><div class="ico"><svg class="icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-linecap="round" stroke-linejoin="round"><path d="M3 3v18h18"/><path d="m19 9-5 5-4-4-3 3"/></svg></div></div><div class="chart-box"><canvas id="chartSOC"></canvas></div></div>
      <div class="card chart-pow"><div class="card-head"><h3>Puissance consommée — 24h</h3><div class="ico"><svg class="icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-linecap="round" stroke-linejoin="round"><path d="M13 2 3 14h9l-1 8 10-12h-9z"/></svg></div></div><div class="chart-box-sm"><canvas id="chartPow"></canvas></div></div>
      <div class="card card-dark kwh-card"><div class="card-head"><h3 style="color:var(--cream)">Consommation du jour</h3><div class="ico"><svg class="icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="4"/><path d="M12 2v2M12 20v2M4.9 4.9l1.4 1.4M17.7 17.7l1.4 1.4M2 12h2M20 12h2M6.3 17.7l-1.4 1.4M19.1 4.9l-1.4 1.4"/></svg></div></div><div><span class="kwh-big" id="kwh-val">--</span><span class="kwh-unit"> kWh</span></div><p style="color:var(--grey-light); font-size:0.82rem; margin-top:8px; font-weight:500;">Énergie totale consommée depuis minuit</p></div>
    </div>
  </div>
  <div class="tab-content" id="journal">
    <div class="grid grid-journal">
      <div class="card logs-card"><div class="card-head"><h3>Journal des événements</h3><button class="btn btn-light" onclick="resetLogs()"><svg class="icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-linecap="round" stroke-linejoin="round"><path d="M3 6h18M19 6v14a2 2 0 0 1-2 2H7a2 2 0 0 1-2-2V6M8 6V4a2 2 0 0 1 2-2h4a2 2 0 0 1 2 2v2"/></svg>Effacer</button></div><div class="log-scroll"><table class="log-table"><thead><tr><th>Horodatage</th><th>Type</th><th>Événement</th></tr></thead><tbody id="log-body"></tbody></table></div></div>
      <div class="card downtime-card"><div class="card-head"><h3>Indisponibilité</h3><div class="ico"><svg class="icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="9"/><path d="M12 7v5l3 2"/></svg></div></div><div class="downtime-big" id="downtime-val">--</div><p style="color:var(--grey); font-size:0.82rem; margin-top:6px; font-weight:500;">Temps total où une charge a été coupée</p></div>
      <div class="card maint-card"><div class="card-head"><h3>Maintenance</h3><div class="ico"><svg class="icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-linecap="round" stroke-linejoin="round"><path d="M14.7 6.3a4 4 0 0 0-5.4 5.4L3 18v3h3l6.3-6.3a4 4 0 0 0 5.4-5.4l-2.8 2.8-2-2z"/></svg></div></div><p style="color:var(--grey); font-size:0.85rem; font-weight:500;">Le mode maintenance suspend les notifications automatiques pendant les interventions.</p><div class="maint-toggle"><span style="font-weight:700; font-size:0.9rem;">Mode maintenance</span><label class="switch"><input type="checkbox" id="maint-sw" onchange="toggleMaintenance(this.checked)"><span class="slider"></span></label></div></div>
    </div>
  </div>
</div>
<nav class="nav-mobile">
  <button class="active" data-tab="dash"><span class="nav-ico"><svg class="icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-linecap="round" stroke-linejoin="round"><rect x="3" y="3" width="7" height="9" rx="1"/><rect x="14" y="3" width="7" height="5" rx="1"/><rect x="14" y="12" width="7" height="9" rx="1"/><rect x="3" y="16" width="7" height="5" rx="1"/></svg></span>Dashboard</button>
  <button data-tab="analyse"><span class="nav-ico"><svg class="icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-linecap="round" stroke-linejoin="round"><path d="M3 3v18h18"/><path d="m19 9-5 5-4-4-3 3"/></svg></span>Analyse</button>
  <button data-tab="journal"><span class="nav-ico"><svg class="icon" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-linecap="round" stroke-linejoin="round"><path d="M4 4h16v16H4z"/><path d="M8 9h8M8 13h8M8 17h5"/></svg></span>Journal</button>
</nav>
<script>
function switchTab(tab){document.querySelectorAll('.tab-content').forEach(c=>c.classList.remove('active'));document.getElementById(tab).classList.add('active');document.querySelectorAll('[data-tab]').forEach(b=>b.classList.toggle('active',b.dataset.tab===tab));}
document.querySelectorAll('[data-tab]').forEach(btn=>btn.addEventListener('click',()=>switchTab(btn.dataset.tab)));
const relayModes={r1:'auto',r2:'auto',r3:'auto'};
function toggleMode(r){
  relayModes[r]=relayModes[r]==='auto'?'manuel':'auto';
  const el=document.getElementById(r+'-mode');
  if(relayModes[r]==='auto'){el.textContent='AUTO';el.classList.add('auto');}
  else{el.textContent='MANUEL';el.classList.remove('auto');}
  fetch('/CMD?MODE_'+r.toUpperCase()+'='+relayModes[r]);
}
function cmdRelay(relay,state){
  if(relayModes[relay.toLowerCase()] === 'auto') { alert('Mode AUTO actif'); fetchData(); return; }
  fetch(`/CMD?${relay}=${state?1:0}`).catch(()=>{});
}
function toggleMaintenance(on){fetch(`/CMD?MAINT=${on?1:0}`).catch(()=>{});}
function resetLogs(){if(confirm('Effacer tout le journal des événements ?')){document.getElementById('log-body').innerHTML='';}}
const labels24h=Array.from({length:24},(_,i)=>`${i}h`);
const chartSOC=new Chart(document.getElementById('chartSOC').getContext('2d'),{type:'line',data:{labels:labels24h,datasets:[{label:'SOC',data:[],borderColor:'#E8C220',backgroundColor:'rgba(244,208,63,0.15)',fill:true,tension:0.4,borderWidth:3,pointRadius:0,pointHoverRadius:5}]},options:{responsive:true,maintainAspectRatio:false,plugins:{legend:{display:false}},scales:{y:{min:0,max:100,grid:{color:'#EDE6D6'},ticks:{color:'#8A8578'}},x:{grid:{display:false},ticks:{color:'#8A8578',maxTicksLimit:12}}}}});
const chartPow=new Chart(document.getElementById('chartPow').getContext('2d'),{type:'line',data:{labels:labels24h,datasets:[{label:'Puissance',data:[],borderColor:'#1C1C1A',backgroundColor:'rgba(28,28,26,0.08)',fill:true,tension:0.4,borderWidth:2.5,pointRadius:0,pointHoverRadius:5}]},options:{responsive:true,maintainAspectRatio:false,plugins:{legend:{display:false}},scales:{y:{beginAtZero:true,grid:{color:'#EDE6D6'},ticks:{color:'#8A8578'}},x:{grid:{display:false},ticks:{color:'#8A8578',maxTicksLimit:12}}}}});
function updateDashboard(data){
  if(data.soc!==undefined){
    document.getElementById('soc-val').textContent=data.soc;
    const ring=document.getElementById('soc-ring');
    const circ=628;
    ring.style.strokeDashoffset=circ-(circ*data.soc/100);
    const st=document.getElementById('soc-state');
    if(data.soc<=10){ring.style.stroke='#E05B4A';st.textContent='Critique';st.className='ring-sub crit';}
    else if(data.soc<=30){ring.style.stroke='#F4D03F';st.textContent='Faible';st.className='ring-sub warn';}
    else{ring.style.stroke='#4CAF6D';st.textContent='Normal';st.className='ring-sub';}
  }
  ['r1','r2','r3'].forEach(r => {
    if(data[r] !== undefined) {
      let isOn = (data[r] == "1");
      let sw = document.getElementById(r+'-sw');
      let st = document.getElementById(r+'-state');
      if(sw) sw.checked = isOn;
      if(st) st.textContent = isOn ? 'Alimenté' : 'Coupé';
      if(sw) sw.disabled = (relayModes[r] === 'auto');
    }
  });
  if(data.vbat!==undefined){document.getElementById('stat-v').textContent=data.vbat;document.getElementById('vbat-mini').textContent=data.vbat+' V';}
  if(data.current!==undefined)document.getElementById('stat-a').textContent=data.current;
  if(data.power!==undefined)document.getElementById('stat-w').textContent=data.power;
  if(data.vac!==undefined)document.getElementById('stat-vac').textContent=data.vac;
  if(data.autonomy!==undefined)document.getElementById('autonomy').textContent=data.autonomy;
  if(data.daily_kwh!==undefined)document.getElementById('kwh-val').textContent=data.daily_kwh;
  if(data.downtime!==undefined)document.getElementById('downtime-val').textContent=data.downtime;
  if(data.history_soc){chartSOC.data.datasets[0].data=data.history_soc;chartSOC.update('none');}
  if(data.history_pow){chartPow.data.datasets[0].data=data.history_pow;chartPow.update('none');}
  if(data.logs)renderLogs(data.logs);
}
function renderLogs(logs){const body=document.getElementById('log-body');body.innerHTML='';const bc={info:'b-info',warn:'b-warn',crit:'b-crit',ok:'b-ok'};const bt={info:'INFO',warn:'ALERTE',crit:'CRITIQUE',ok:'OK'};logs.forEach(l=>{const tr=document.createElement('tr');tr.innerHTML=`<td class="log-time">${l.time}</td><td><span class="log-badge ${bc[l.type]||'b-info'}">${bt[l.type]||'INFO'}</span></td><td>${l.msg}</td>`;body.appendChild(tr);});}
function fetchData(){fetch('/DATA').then(r=>r.json()).then(data=>{updateDashboard(data);document.getElementById('conn-dot').className='dot';document.getElementById('conn-text').textContent='En ligne';}).catch(()=>{document.getElementById('conn-dot').className='dot off';document.getElementById('conn-text').textContent='Hors ligne';});}
fetchData();setInterval(fetchData,3000);
</script>
</body>
</html>
)rawliteral";