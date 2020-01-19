#ifndef _AIMBOT_
#define _AIMBOT_

static BYTE RageBoneFront[7] = { {1},{2},{3},{4},{5},{6},{7}, };
static BYTE RageBoneBack1[14] = { {10},{11},{12},{25},{26},{27},{48},{42},{43},{44},{45},{49},{50},{51}, };
static BYTE RageBoneBack2[14] = { {10},{11},{12},{24},{25},{26},{46},{40},{41},{42},{43},{47},{48},{49}, };

static BYTE TriggerBone[4] = { {1},{3},{4},{6}, };
static BYTE TriggerBone1[18] = { {1},{3},{4},{6},{10},{11},{12},{25},{26},{27},{48},{42},{43},{44},{45},{49},{50},{51}, };
static BYTE TriggerBone2[18] = { {1},{3},{4},{6},{10},{11},{12},{24},{25},{26},{46},{40},{41},{42},{43},{47},{48},{49}, };

static BYTE LegitBone[4] = { {3},{4},{5},{6}, };
static BYTE LegitBone1[14] = { {10},{11},{12},{25},{26},{27},{48},{42},{43},{44},{45},{49},{50},{51}, };
static BYTE LegitBone2[14] = { {10},{11},{12},{24},{25},{26},{46},{40},{41},{42},{43},{47},{48},{49}, };

extern DWORD dwReactionTime;

extern int iTargetLegit;
extern int iTargetRage;
extern int iTargetKnife;

void AimBot(float frametime, struct usercmd_s* cmd);
void DrawAimBotFOV();
void AntiAim(struct usercmd_s* cmd);
void FastZoom(struct usercmd_s* cmd);
void RageDraw();
void TriggerDraw();
void LegitDraw();
void KnifeDraw();
void UpdateAimbot(float frametime);
void ContinueFire(usercmd_s* cmd);
bool FakeEdge(float& angle);
void DrawAimbot();

#endif