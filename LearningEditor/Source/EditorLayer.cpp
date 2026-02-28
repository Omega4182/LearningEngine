
#include "EditorLayer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace LE
{
    class TestShared : public SharedFromThis
    {
    public:
        TestShared()
        {
            LE_INFO("Shared created");
        }

        virtual ~TestShared()
        {
            LE_INFO("Shared destroyed");
        }

    private:
        uint32_t some_info = 0;
    };

    class TestSharedChild : public TestShared
    {
    public:
        TestSharedChild()
        {
            LE_INFO("Shared Child created");
        }

        virtual ~TestSharedChild()
        {
            LE_INFO("Shared Child destroyed");
        }

    private:
        uint32_t some_info_child = 0;
    };

    EditorLayer::EditorLayer()
        : Layer("EditorLayer"), m_CameraController(1280.f / 720.f)
    {
    }

    void EditorLayer::OnAttach()
    {
        LE_PROFILE_FUNCTION();

        m_Texture = Texture2D::Create("Assets/Textures/Checkerboard.png");

        FramebufferSpecification frameSpec;
        frameSpec.Width = 1280;
        frameSpec.Height = 720;
        m_Framebuffer = Framebuffer::Create(frameSpec);

        m_ActiveScene = MakeShared<Scene>();

        Entity SquareEntity = m_ActiveScene->CreateEntity();
        SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4(0.f, 1.f, 0.f, 1.f));

        //SharedPtr<TestShared> NewTestShared = MakeShared<TestShared>();
        //SharedPtr<TestShared> NewTestShared1 = NewTestShared;

        //NewTestShared = SharedPtr<TestShared>(new TestShared());
    }

    void EditorLayer::OnDetach()
    {
        LE_PROFILE_FUNCTION();
    }

    void EditorLayer::OnUpdate(Timestep DeltaTime)
    {
        LE_PROFILE_FUNCTION();

        // Resize
        {
            FramebufferSpecification spec = m_Framebuffer->GetSpecification();
            if (m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
                (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
            {
                m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
                m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
            }
        }

        if (m_ViewportFocused)
        {
            m_CameraController.OnUpdate(DeltaTime);
        }

        Renderer2D::ResetStats();

        m_Framebuffer->Bind();
        RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
        RenderCommand::Clear();

        Renderer2D::BeginScene(m_CameraController.GetCamera());
        m_ActiveScene->OnUpdate(DeltaTime);
        //Renderer2D::DrawRotatedQuad(glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 1.f), glm::radians(45.f), glm::vec4(1.f, 0.f, 0.f, 1.f));

        /*for (uint32_t i = 0; i < 100; i++)
        {
            uint32_t row = i / 10;
            uint32_t column = i % 10;

            Renderer2D::DrawQuad(glm::vec3(row * 2.f, column * 2.f, 0.f), glm::vec2(1.f, 1.f), glm::vec4(row / 10.f, column / 10.f, row / 10.f, 1.f));
        }*/
        /*Renderer2D::DrawQuad(glm::vec3(-0.5f, -0.5f, 0.f), glm::vec2(1.f, 1.f), m_Texture, 20.f, glm::vec4(1.f, 1.f, 1.f, 1.f));
        Renderer2D::DrawQuad(glm::vec3(0.f, 0.f, 0.f), glm::vec2(1.f, 1.f), glm::vec4(1.f, 0.f, 0.f, 1.f));

        static float TestRot = 0.f;
        TestRot += DeltaTime * 20.f;
        Renderer2D::DrawRotatedQuad(glm::vec3(1.2f, 1.2f, 0.f), glm::vec2(1.f, 1.f), TestRot, glm::vec4(0.5f, 0.5f, 1.f, 1.f));
        Renderer2D::DrawRotatedQuad(glm::vec3(-1.2f, -1.2f, 0.f), glm::vec2(1.f, 1.f), TestRot, glm::vec4(0.5f, 0.5f, 1.f, 1.f));

        Renderer2D::DrawRotatedQuad(glm::vec3(0.0f, 0.0f, 0.f), glm::vec2(10.f, 10.f), 45.f, m_Texture, 10.f, glm::vec4(1.f, 1.f, 1.f, 1.f));*/

        Renderer2D::EndScene();

        m_Framebuffer->Unbind();
    }

    void EditorLayer::OnImGuiRender()
    {
        LE_PROFILE_FUNCTION();

        static bool bDockspaceOpen = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &bDockspaceOpen, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Options"))
            {
                if (ImGui::MenuItem("Exit")) { Application::Get().Close(); }

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::Begin("Settings");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        Renderer2D::Statistics Stats = Renderer2D::GetStats();
        ImGui::Text("Renderer 2D Stats:");
        ImGui::Text("Draw Calls: %d", Stats.DrawCalls);
        ImGui::Text("Quads count: %d", Stats.QuadCount);
        ImGui::Text("Vertices count: %d", Stats.GetTotalVertexCount());
        ImGui::Text("Indices count: %d", Stats.GetTotalIndexCount());

        ImGui::ColorPicker4("SquareColor", &m_SquareColor[0]);
        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Viewport");

        m_ViewportFocused = ImGui::IsWindowFocused();
        m_ViewportHovered = ImGui::IsWindowHovered();
        Application::Get().GetImGuiLayer()->SetBlockEvents(m_ViewportFocused == false || m_ViewportHovered == false);

        ImVec2 ViewportSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = glm::vec2(ViewportSize.x, ViewportSize.y);

        uint32_t TextureId = m_Framebuffer->GetColorAttachmentRendererId();
        ImGui::Image(TextureId, ViewportSize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();
    }

    void EditorLayer::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        m_CameraController.OnEvent(e);
    }
}