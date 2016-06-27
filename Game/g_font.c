void PrintText(GameState* state, char* text, float* x, float* y)
{
   stbtt_bakedchar* cdata = state->cdata;

   Buffer buffer = {};
   Mesh mesh = {};

   float vertices[8];
   float uvs[8];
   int indices[6];

   u32 index = 0;

   int i = 0;
   while(*text) 
   {
      if (*text >= 32 && *text < 128) 
      {
         //u32 vao;

         stbtt_aligned_quad q;
         stbtt_GetBakedQuad(cdata, 512, 512, *text-32, x, y, &q, 1);

         uvs[2] = q.s1; uvs[3] = q.t0; vertices[0] = q.x0; vertices[1] = q.y0;
         uvs[0] = q.s0; uvs[1] = q.t0; vertices[2] = q.x1; vertices[3] = q.y0;
         uvs[6] = q.s0; uvs[7] = q.t1; vertices[4] = q.x1; vertices[5] = q.y1;
         uvs[4] = q.s1; uvs[5] = q.t1; vertices[6] = q.x0; vertices[7] = q.y1;

         indices[0] = 0; indices[1] = 1; indices[2] = 3; 
         indices[3] = 2; indices[4] = 3; indices[5] = 1; 

         //Add Font

         ++i;     
      }
      ++text;
   }
}