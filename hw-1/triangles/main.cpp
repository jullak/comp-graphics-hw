#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <util/window_initializer.h>

#include <util/buffers.h>
#include <util/camera.h>
#include <util/shader.h>
#include <util/model.h>

using namespace glm;


namespace {
    class MyScenario : public CameraRotation::IScenario {
    public:
        MyScenario() {
            x = r * sin(M_PI * theta / 360) * cos(M_PI * phi / 720);
            y = r * sin(M_PI * theta / 360) * sin(M_PI * phi / 720);
            z = r * cos(M_PI * theta / 360);
        }
        ~MyScenario() override = default;

        glm::vec3 getStartPosition() const override {
            return glm::vec3(3 * sin(M_PI * 0 / 360) * cos(M_PI * 0 / 720),
                             3 * sin(M_PI * 0 / 360) * sin(M_PI * 0 / 720),
                             3 * cos(M_PI * 0 / 360));
        }
        glm::vec3 getNewPosition() override {
            ++theta;
            x = r * sin(M_PI * theta / 360) * cos(M_PI * phi / 720);
            y = r * sin(M_PI * theta / 360) * sin(M_PI * phi / 720);
            z = r * cos(M_PI * theta / 360);

            return glm::vec3(x, y, z);
        }

    private:
        float r = 3;
        float theta = 0;
        float phi = 120;

        float x;
        float y;
        float z;
    };
}

int main() {
    WindowInitializer init;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint program_id_1 = Shader::LoadShaders( "VertexShader.glsl", "FragmentShader1.glsl" );
    GLuint program_id_2 = Shader::LoadShaders( "VertexShader.glsl", "FragmentShader2.glsl" );

    Model triangle1;
    Model triangle2;
    triangle1.AddTriangle(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(-0.25f, 0.0f, 0.0f));
    triangle2.AddTriangle(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3( -0.5f, 0.5f, 0.0f), glm::vec3(0.25f, 0.0f, 0.0f));

    std::vector<GLfloat> global_vertex_buffer;
    triangle1.PointsToBuffer(global_vertex_buffer);
    triangle2.PointsToBuffer(global_vertex_buffer);
    Buffers::VBOBindScoped vertex_vbo(global_vertex_buffer);


    MyScenario scenario;
    CameraRotation::Camera cam(glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f),
                               glm::vec3(0, 0, 0), // Camera is at (4,3,3), in World Space
                               glm::vec3(0, 0, 0), // and looks at the origin
                               glm::vec3(0, 1, 0),  // Head is up (set to 0,-1,0 to look upside-down));
                               glm::mat4(1.0f),
                               static_cast<CameraRotation::IScenario*>(&scenario));


    GLuint matrix_id_1 = glGetUniformLocation(program_id_1, "MVP");
    GLuint matrix_id_2 = glGetUniformLocation(program_id_2, "MVP");

    do {
        // Clear the screen
        glClear( GL_COLOR_BUFFER_BIT );
        {
            Buffers::VAOEnableScoped scoped_vertex(0);

            glVertexAttribPointer(
                    scoped_vertex.GetId(),     // attribute 0. No particular reason for 0, but must match the layout in the shader.
                    3,                        // size
                    GL_FLOAT,                 // type
                    GL_FALSE,                 // normalized?
                    0,                        // stride
                    (void *) 0                // array buffer offset
            );

            glUseProgram(program_id_1);
            glUniformMatrix4fv(matrix_id_1, 1, GL_FALSE, &cam.GetMVP()[0][0]);
            glDrawArrays(GL_TRIANGLES, triangle1.GetPosPoint(),  triangle1.TriangleCount() * 3);

            glUseProgram(program_id_2);
            glUniformMatrix4fv(matrix_id_2, 1, GL_FALSE, &cam.GetMVP()[0][0]);
            glDrawArrays(GL_TRIANGLES, triangle2.GetPosPoint(),  triangle2.TriangleCount() * 3);
        }

        cam.RebuildMVP();
        // Swap buffers
        glfwSwapBuffers(init.GetWindowPtr());
        glfwPollEvents();
    } while (glfwGetKey(init.GetWindowPtr(), GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(init.GetWindowPtr()) == 0);

    glDeleteProgram(program_id_1);
    glDeleteProgram(program_id_2);

    return 0;
}
