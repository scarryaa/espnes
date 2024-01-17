#include "../include/window.hpp"
#include "../include/debug/disassembler.hpp"
#include "../include/emulator.hpp"

Window::Window()
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    }

    window = SDL_CreateWindow("espnes", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 256, 240);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();

    // Make the window resizable
    SDL_SetWindowResizable(this->window, SDL_TRUE);

    // Initialize SDL Renderer for ImGui
    ImGui_ImplSDL2_InitForSDLRenderer(this->window, this->renderer);
    ImGui_ImplSDLRenderer2_Init(this->renderer);
}

Window::~Window()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Window::poll_events()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);

        if (event.type == SDL_QUIT)
        {
            return true;
        }
    }

    return false;
}

void Window::render_disassembly(Emulator* emulator)
{
    CPU* cpu = emulator->get_CPU(); // Get the CPU from the emulator
    Disassembler disassembler = emulator->get_disassembler();
    uint16_t pc = cpu->get_PC();     // Get the current PC from the CPU
    uint16_t startAddress = pc - 10; // Start disassembling a few instructions before the PC for context

    ImGui::Begin("Disassembler");

    // Flags (highlighted if set NVUBDIZC)
    if (cpu->get_N())
    {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "N");
    }
    else
    {
        ImGui::Text("N");
    }
    ImGui::SameLine();

    if (cpu->get_V())
    {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "V");
    }
    else
    {
        ImGui::Text("V");
    }
    ImGui::SameLine();

    if (cpu->get_U())
    {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "U");
    }
    else
    {
        ImGui::Text("U");
    }
    ImGui::SameLine();

    if (cpu->get_B())
    {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "B");
    }
    else
    {
        ImGui::Text("B");
    }
    ImGui::SameLine();

    if (cpu->get_D())
    {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "D");
    }
    else
    {
        ImGui::Text("D");
    }
    ImGui::SameLine();

    if (cpu->get_I())
    {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "I");
    }
    else
    {
        ImGui::Text("I");
    }
    ImGui::SameLine();

    if (cpu->get_Z())
    {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Z");
    }
    else
    {
        ImGui::Text("Z");
    }
    ImGui::SameLine();

    if (cpu->get_C())
    {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "C");
    }
    else
    {
        ImGui::Text("C");
    }

    // Registers
    // A
    ImGui::Text("A: %02X", cpu->get_A());
    ImGui::SameLine();

    // X
    ImGui::Text("X: %02X", cpu->get_X());
    ImGui::SameLine();

    // Y
    ImGui::Text("Y: %02X", cpu->get_Y());
    ImGui::SameLine();

    // SP
    ImGui::Text("SP: %02X", cpu->get_SP());

    // PC
    ImGui::Text("PC: %04X", cpu->get_PC());
    ImGui::SameLine();

    // P
    ImGui::Text("P: %02X", cpu->get_P());

    // Buttons
    if (ImGui::Button("Step"))
    {
        emulator->step();
    }

    ImGui::SameLine();

    if (ImGui::Button("Reset"))
    {
        emulator->reset();
    }

    ImGui::SameLine();

    if (emulator->is_paused())
    {
        if (ImGui::Button("Resume"))
        {
            emulator->pause();
        }
    }
    else
    {
        if (ImGui::Button("Pause"))
        {
            emulator->pause();
        }
    }

    ImGui::BeginChild("scrolling_region", ImVec2(0, 0), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);
    ImGui::Columns(6, "disassembler_columns", false);
    ImGui::SetColumnWidth(0, 15.0f);
    ImGui::SetColumnWidth(1, 50.0f);

    uint16_t address = startAddress;
    for (int i = 0; i < 20; i++)
    {
        // Check if the address is a breakpoint
        // if so, draw a red circle
        if (emulator->is_breakpoint(BREAKPOINT_TYPE_ADDRESS, address))
        {
            ImGui::SetCursorPosX(5.0f);
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "O");
        }
        else
        {
            ImGui::Text(" ");
        }
        ImGui::NextColumn();

        Disassembler::Instruction instruction = disassembler.disassemble(address);
        if (address == pc)
        {
            // Highlight the current PC
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%04X", instruction.address);
        }
        else
        {
            ImGui::Text("%04X", instruction.address);
        }

        // Check for double click to set a breakpoint
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
        {
            // Toggle the breakpoint
            if (emulator->is_breakpoint(BREAKPOINT_TYPE_ADDRESS, address))
            {
                emulator->clear_breakpoint(BREAKPOINT_TYPE_ADDRESS, address);
            }
            else
            {
                emulator->add_breakpoint(BREAKPOINT_TYPE_ADDRESS, address);
            }
        }

        ImGui::NextColumn();

        // Opcode
        ImGui::Text("%02X", instruction.opcode);
        ImGui::NextColumn();

        // Op 1
        ImGui::Text(instruction.bytes[0] == ' ' ? " " : "%02X", instruction.bytes[0]);
        ImGui::NextColumn();

        // Op 2
        ImGui::Text(instruction.bytes[1] == ' ' ? " " : "%02X", instruction.bytes[1]);
        ImGui::NextColumn();

        // Mnemonic
        ImGui::Text("%s", instruction.mnemonic);
        ImGui::NextColumn();
        address += instruction.length;
    }

    ImGui::EndChild();

    ImGui::End();
}

void Window::render_memory_view(Emulator* emulator)
{
    PPU* ppu = emulator->get_PPU();
    uint8_t* vram = ppu->get_vram();

    ImGui::Begin("VRAM View");

    const int bytes_per_row = 16;
    int total_rows = 0x2000 / bytes_per_row;

    ImGui::BeginChild("VRAM", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

    for (int row = 0; row < total_rows; ++row)
    {
        for (int col = 0; col < bytes_per_row; ++col)
        {
            int addr = row * bytes_per_row + col;
            ImGui::Text("%02X ", vram[addr]);
            ImGui::SameLine();
        }
        ImGui::NewLine();
    }

    ImGui::EndChild();
    ImGui::End();
}

void Window::render(Emulator* emulator)
{
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame(this->window);
    ImGui::NewFrame();

    this->render_menu_bar(*emulator);

    CPU* cpu = emulator->get_CPU();
    if (this->show_disassembly)
    {
        this->render_disassembly(emulator);
    }

    this->render_PPU(emulator);
    this->render_CPU(emulator);
    this->render_memory_view(emulator);
    this->render_breakpoints(emulator);
}

void Window::render_PPU(Emulator* emulator)
{
    PPU* ppu = emulator->get_PPU();
    ImGui::Begin("PPU");

    ImGui::Text("Scanline: %d", ppu->get_scanline());
    ImGui::Text("Cycle: %d", ppu->get_cycle());
    ImGui::Text("Frame: %d", ppu->get_frame());

    ImGui::End();
}

void Window::render_CPU(Emulator* emulator)
{
    CPU* cpu = emulator->get_CPU();
    ImGui::Begin("CPU");

    ImGui::Text("Cycle: %d", cpu->get_cycles());

    ImGui::End();
}

void Window::render_breakpoints(Emulator* emulator)
{
    ImGui::Begin("Breakpoints");

    ImGui::BeginChild("scrolling_region", ImVec2(0, 0), false);

    auto breakpoints = emulator->get_breakpoints();
    for (auto it = breakpoints.begin(); it != breakpoints.end(); ++it)
    {
		ImGui::Text("%04X", *it);
	}

    ImGui::EndChild();

    ImGui::Button("Clear All");

    if (ImGui::IsItemClicked())
    {
		emulator->clear_all_breakpoints();
	}

    ImGui::SameLine();

    ImGui::Button("Add");

    // Spawn a new window if the button is clicked
    if (ImGui::IsItemClicked())
    {
        ImGui::OpenPopup("Add Breakpoint");
    }

    // Create the popup window
    if (ImGui::BeginPopup("Add Breakpoint"))
    {
		static char input[5] = "0000";
		ImGui::InputText("Address", input, 5);
        
        // Radio button for scanline breakpoint
        static bool scanline = false;
        ImGui::RadioButton("Scanline", scanline);

        static bool cycle = false;
        static bool frame = false;
        static bool instruction = false;
        static bool read = false;
        static bool write = false;
        static bool execute = false;

        if (ImGui::IsItemClicked())
        {
            scanline = true;

            // Clear the other radio buttons
            cycle = false;
            frame = false;
            instruction = false;
            read = false;
            write = false;
            execute = false;
		}

        // Radio button for cycle breakpoint
        ImGui::RadioButton("Cycle", cycle);

        if (ImGui::IsItemClicked())
        {
			cycle = true;

            // Clear the other radio buttons
			scanline = false;
			frame = false;
			instruction = false;
			read = false;
			write = false;
			execute = false;
        }

        // Radio button for frame breakpoint
        ImGui::RadioButton("Frame", frame);

        // Radio button for instruction breakpoint
        ImGui::RadioButton("Instruction", instruction);

        // Radio button for read breakpoint
        ImGui::RadioButton("Read", read);

        // Radio button for write breakpoint
        ImGui::RadioButton("Write", write);

        // Radio button for execute breakpoint
        ImGui::RadioButton("Execute", execute);

        // Add the breakpoint
        if (ImGui::Button("Add"))
        {
			// Convert the input to an integer
			uint16_t value = std::stoi(input, nullptr, 16);

            breakpoint_type_t type;

            if (cycle)
            {
				type = BREAKPOINT_TYPE_CYCLE;
			}
            else
            {
				type = BREAKPOINT_TYPE_ADDRESS;
			}

			emulator->add_breakpoint(type, value);

			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
    }

	ImGui::End();
}

void Window::render_menu_bar(Emulator& emulator)
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open ROM"))
            {
                std::cout << "Open ROM" << std::endl;
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Debug"))
        {
            if (ImGui::MenuItem("Disassembly"))
            {
                // Toggle the disassembly window
                this->show_disassembly = !this->show_disassembly;
                ImGui::Checkbox("Show Disassembly", &this->show_disassembly);
            }

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void Window::post_render(uint8_t* frame_buffer)
{
    // // Delay to control frame rate (16 ms = 60 fps)
    // SDL_Delay(16);

    ImGui::Render();
    SDL_RenderClear(this->renderer);
    SDL_UpdateTexture(this->texture, nullptr, frame_buffer, 256 * 4);
    SDL_RenderCopy(this->renderer, this->texture, nullptr, nullptr);
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(this->renderer);
}