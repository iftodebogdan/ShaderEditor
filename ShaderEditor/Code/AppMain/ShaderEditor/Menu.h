#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <iostream>
#include <gtkmm.h>
#include "Page.h"
#include "Renderer.h"
using namespace LibRendererDll;

/**
 * Class used for building the menu from the workspace frame.
 */
class MenuWidget
{
public:
	// The class constructors.
	MenuWidget();
	MenuWidget(
		Gtk::Window*										window,
		Gtk::Frame*											container,
		std::map<int, std::map<std::string, std::string>>	items,
		Gtk::Notebook*										notebook
	);
	// The class destructor.
	virtual ~MenuWidget();

	void UpdatePassTable();
	void UpdateConstantTable(unsigned int pass, std::vector<ShaderInputDesc>& constantTable);
	void OnPassItemActivated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* /* column */);
	void OnConstantItemActivated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* /* column */);
	void OnPassWindowMenuPopup(GdkEventButton* ev);
	void OnPassAddAbove();
	void OnPassAddBelow();
	void OnPassRemove();
	void OnRenderTargetAdd();
	void OnRenderTargetRemove();

	struct ShaderPassDesc
	{
		ShaderPassDesc() { model = nullptr; }

		std::string name;
		VertexBuffer* model;
		
		struct ShaderProgram
		{
			ShaderProgram() { program = nullptr; }
			std::string src;
			ShaderTemplate* program;

		} vertexShader, pixelShader;

		struct ShaderConstants
		{
			ShaderConstants() { for (int i = 0; i < 16; i++) valueFloat[i] = 0.f; }
			std::string name;
			InputType inputType;
			float valueFloat[16];
			unsigned int rows, cols, sizeOfArray;
			struct TextureDesc
			{
				TextureDesc() { boundTexture = nullptr; }
				Texture* boundTexture;
				SamplerStateDesc samplerStates;
			} sampler;
		};
		std::vector<ShaderConstants> constants;

		struct Camera
		{
			Vec3f pos;
			float fovYDeg;
			float zNear;
			float zFar;
		} camera;

		RenderTarget* renderTarget;

		struct RenderState
		{
			// Alpha states
			bool			m_bAlphaBlendEnable;
			Blend			m_eAlphaSrcBlend;
			Blend			m_eAlphaDstBlend;
			bool			m_bAlphaTestEnable;
			Cmp				m_eAlphaFunc;
			float			m_fAlphaRef;
			Vec4f			m_vBlendFactor;

			// Culling states
			Cull			m_eCullMode;

			// Depth states
			ZBuffer			m_eZEnable;
			Cmp				m_eZFunc;
			bool			m_bZWriteEnable;

			// Depth bias states
			float			m_fSlopeScaleDepthBias;
			float			m_fDepthBias;

			// Stencil states
			bool			m_bStencilEnable;
			Cmp				m_eStencilFunc;
			unsigned long	m_lStencilRef;
			unsigned long	m_lStencilMask;
			unsigned long	m_lStencilWriteMask;
			StencilOp		m_eStencilFail;
			StencilOp		m_eStencilZFail;
			StencilOp		m_eStencilPass;

			// Fill mode
			Fill			m_eFillMode;
		} renderStates;
	};
	// Lazy man's accessor
	static std::vector<ShaderPassDesc>					m_arrShaderPassDesc;
protected:

	Gtk::Window*										m_Window;
	Gtk::Frame*											m_Container;
	std::map<int, std::map<std::string, std::string>>	m_Items;
	Glib::RefPtr<Gtk::ActionGroup>						m_ActionGroup;
	Glib::RefPtr<Gtk::UIManager>						m_UIManager;
	Gtk::Notebook*										m_Notebook;
	std::map<std::string, Gtk::VBox*>					m_Pages;
	Gtk::Notebook										m_WorkspaceNotebook;

	Gtk::TreeModel::ColumnRecord						m_TableColumnRecord;
	Gtk::TreeModelColumn<std::string>					m_TableColumn;

	Glib::RefPtr<Gtk::TreeStore>						m_PassTableTreeModel;
	Gtk::TreeView										m_PassTable;
	std::vector<Gtk::TreeModel::Row>					m_arrPassRow;
	std::vector<Gtk::TreeModel::Row>					m_arrConstantRow;
	Gtk::Menu											m_PassPopupMenu;
	Gtk::Dialog											m_PassAddDialog;
	Gtk::Entry											m_PassAddName;

	Glib::RefPtr<Gtk::TreeStore>						m_ConstantTableTreeModel;
	Gtk::TreeView										m_ConstantTable;
	Gtk::TreeModel::Row									m_SamplersRow;
	Gtk::TreeModel::Row									m_FloatsRow;

	// The menu builder method.
	void buildMenu();
	// The menu item action callback.
	void onMenuAccess(std::string name, std::string filename);
	// Helper method used for fetching the index of page, given its name.
	int MenuWidget::getPageIndexByName(std::string name);
};

#endif