
#include "g_camera.h"

void AllocateRenderer(Renderer* renderer, u32 num)
{
	//renderer->ui.entries = (EntryUI*)GetMemory(num * sizeof(EntryUI));
	//renderer->model.entries = (EntryModel*)GetMemory(num * sizeof(EntryModel));


	renderer->ui.num_entries = 0;
	renderer->model.num_entries = 0;
}

void Start(Renderer* renderer)
{
	renderer->ui.num_entries = 0;
	renderer->model.num_entries = 0;
}

GLint GetMatrix4(GLuint program, const char* name)
{
    return glGetUniformLocation(program, name);
}

void SetMatrix4(GLint id, M4 value)
{ 
    glUniformMatrix4fv(id, 1, GL_FALSE, value.data);
}

void Render(Renderer* unit)
{
	//GLuint view, projection, modelm, program, light_position;
	UIRenderer* uis = &unit->ui;
	ModelRenderer* models = &unit->model; 

	glDisable(GL_DEPTH_TEST);

	{
		u32 model, proj, color;

		int num_entries = uis->num_entries;
		EntryUI* entry = uis->entries;

		M4 m;

		for(int i = 0; i < num_entries; ++i)
		{
			u32 program = uis->ui_program;

			glBindVertexArray(entry->vao);

			TurnOnShader(program);

			GetTranslationMatrix(&m, {entry->offset.x, entry->offset.y, 0.0f});

			model = GetMatrix4(program, "model");
			proj = GetMatrix4(program, "projection");
			color = GetMatrix4(program, "color");

			SetMatrix4(model, m);
			SetMatrix4(proj, unit->orthographic);
			glUniform3f(color, entry->color.x, entry->color.y, entry->color.z);
			//SetMatrix4(proj, unit->perspective);


			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			++entry;
		}
	}

	glEnable(GL_DEPTH_TEST);

	{
		int num_entries = models->num_entries;
		EntryModel* entry = models->entries;

		for(int i = 0; i < num_entries; ++i)
		{

			glDrawElements(GL_TRIANGLES, entry->num_indices, GL_UNSIGNED_INT, 0);

			++entry;
		}
	}
}
