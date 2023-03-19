#include "./Skybox.h"

namespace renderer {
    Skybox::Skybox(float size) : m_SkyboxTexture("skybox"), m_SkyboxShader("skybox") {
        this->m_SkyboxShader.Reload();

        float neg = -size;
        float pos = size;

        float vertices[]{
                neg, neg, neg, 0.0f, 0.5f, //-Z
                neg, pos, neg, 0.0f, 0.0f, //-Z
                pos, pos, neg, 0.5f, 0.0f, //-Z
                pos, neg, neg, 0.5f, 0.5f, //-Z

                neg, neg, pos, 0.0f, 0.5f, //+Z
                neg, pos, pos, 0.0f, 0.0f, //+Z
                pos, pos, pos, 0.5f, 0.0f, //+Z
                pos, neg, pos, 0.5f, 0.5f, //+Z

                neg, neg, neg, 0.0f, 0.5f, //Bottom
                neg, neg, pos, 0.0f, 1.0f, //Bottom
                pos, neg, pos, 0.5f, 1.0f, //Bottom
                pos, neg, neg, 0.5f, 0.5f, //Bottom

                neg, pos, neg, 0.5f, 0.0f, //Top
                neg, pos, pos, 0.5f, 0.5f, //Top
                pos, pos, pos, 1.0f, 0.5f, //Top
                pos, pos, neg, 1.0f, 0.0f, //Top

                pos, neg, neg, 0.0f, 0.5f, //+X
                pos, pos, neg, 0.0f, 0.0f, //+X
                pos, pos, pos, 0.5f, 0.0f, //+X
                pos, neg, pos, 0.5f, 0.5f, //+X

                neg, neg, neg, 0.0f, 0.5f, //-X
                neg, pos, neg, 0.0f, 0.0f, //-X
                neg, pos, pos, 0.5f, 0.0f, //-X
                neg, neg, pos, 0.5f, 0.5f, //-X
        };

        unsigned short indices[]{
                //-z
                0, 1, 2,
                0, 2, 3,

                //+z
                4, 5, 6,
                4, 6, 7,

                //bottom
                8, 9, 10,
                8, 10, 11,

                //top
                12, 13, 14,
                12, 14, 15,

                //+x
                16, 17, 18,
                16, 18, 19,

                //-x
                20, 21, 22,
                20, 22, 23,
        };

        this->m_IndexCount = sizeof(indices) / 2;

        glGenVertexArrays(1, &this->VAO);

        glGenBuffers(1, &this->VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &indexVBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void Skybox::Render(glm::mat4 projMat) {
        this->m_SkyboxShader.Bind();
        this->m_SkyboxTexture.Bind();
        this->m_SkyboxShader.SetUniformMat4("projectionMatrix", projMat);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
        glDrawElements(
                GL_TRIANGLES,
                this->m_IndexCount,
                GL_UNSIGNED_SHORT,
                nullptr
        );

        glBindVertexArray(0);
        this->m_SkyboxTexture.UnBind();
        this->m_SkyboxShader.UnBind();
    }
} // namespace rederer

