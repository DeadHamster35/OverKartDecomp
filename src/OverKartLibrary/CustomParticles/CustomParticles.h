extern void InitialCustomParticleBuffer(void);
extern short AddCustomParticleBuffer(Vector pos,SVector Angle,Vector velo,float Scale,short Category,char filtermode);
extern void DeleteCustomParticleBuffer(CustomParticle *particle);

extern short ParticleCreateLocal(short Category, int Player, bool screen_share, short Angle, Vector pos, float Scale, float speed_forward, float speed_up, short rotationadd, Ushort rndoffset, char filtermode, Uchar anim_speed, Uchar Loopcount, Uchar Xflip, Uint RGBA);

extern void MoveCustomParticle(void);
extern void DrawCustomParticle(int Player);
