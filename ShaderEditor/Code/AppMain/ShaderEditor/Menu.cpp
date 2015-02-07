#include "stdafx.h"
#include "Menu.h"

std::vector<MenuWidget::ShaderPassDesc> MenuWidget::m_arrShaderPassDesc;

/**
 * The default class constructor.
 */
MenuWidget::MenuWidget()
{
}

/**
 * The real class constructor.
 */
MenuWidget::MenuWidget(
	Gtk::Window* window,
	Gtk::Frame* container,
	std::map<int, std::map<std::string, std::string>> items,
	Gtk::Notebook* notebook
) {
	// Initialise properties.
	this->m_Window = window;
	this->m_Container = container;
	this->m_Items = items;
	this->m_ActionGroup = Gtk::ActionGroup::create();
	this->m_UIManager = Gtk::UIManager::create();
	this->m_Notebook = notebook;

	// Build the menu.
	this->buildMenu();
}

/**
 * The class destructor.
 */
MenuWidget::~MenuWidget()
{
}

/**
 * The menu builder method.
 */
void MenuWidget::buildMenu()
{
	//Gtk::Widget* menuBar;
	//std::string name;
	//std::map<int, std::map<std::string, std::string>>::iterator iterator;
	//Glib::ustring ui_string;
	//
	//// Build the action group and the UI from the given items.
	//for (iterator = this->m_Items.begin(); iterator != this->m_Items.end(); iterator++) {
	//	name = iterator->second["name"];
	//
	//	// On menu button click, we want to open a new tab into the notebook.
	//	this->m_ActionGroup->add(
	//		Gtk::Action::create(name, name),
	//		sigc::bind(sigc::mem_fun(*this, &MenuWidget::onMenuAccess), name, iterator->second["file"])
	//		);
	//
	//	ui_string += "<toolitem action='" + name + "' />";
	//}
	//
	//// Set action group.
	//this->m_UIManager->insert_action_group(this->m_ActionGroup);
	//
	//// Load the defined UI.
	//ui_string =
	//	"<ui>"
	//	"  <toolbar name='MenuBar'>"
	//	+ ui_string +
	//	"  </toolbar>"
	//	"</ui>";
	//this->m_UIManager->add_ui_from_string(ui_string);
	//
	//// Fetch the menu bar and orient it vertically.
	//menuBar = this->m_UIManager->get_widget("/MenuBar");
	//menuBar->set_property("orientation", Gtk::ORIENTATION_VERTICAL);
	//
	//// Add the menu bar and the accel_group to the window.
	//this->m_Window->add_accel_group(this->m_UIManager->get_accel_group());
	//this->m_Container->add(m_WorkspaceNotebook);

	// Create the passes workspace item
	m_TableColumnRecord.add(m_TableColumn);

	m_PassTableTreeModel = Gtk::TreeStore::create(m_TableColumnRecord);
	m_PassTable.set_model(m_PassTableTreeModel);
	
	m_PassTable.append_column("Passes", m_TableColumn);
	m_PassTable.set_headers_visible(false);
	m_PassTable.signal_row_activated().connect(sigc::mem_fun(*this, &MenuWidget::OnPassItemActivated));

	m_PassTable.add_events(Gdk::BUTTON_PRESS_MASK);
	m_PassTable.signal_button_press_event().connect_notify(sigc::mem_fun(*this, &MenuWidget::OnPassWindowMenuPopup));
	
	// Create the constant table workspace item
	m_ConstantTableTreeModel = Gtk::TreeStore::create(m_TableColumnRecord);
	m_ConstantTable.set_model(m_ConstantTableTreeModel);

	m_ConstantTable.append_column("Constant Table", m_TableColumn);
	m_ConstantTable.set_headers_visible(false);
	m_ConstantTable.signal_row_activated().connect(sigc::mem_fun(*this, &MenuWidget::OnConstantItemActivated));

	// Build workspace notebook
	m_WorkspaceNotebook.append_page(m_PassTable, "Passes");
	m_WorkspaceNotebook.append_page(m_ConstantTable, "Constants");

	// Populate popup menu
	Gtk::MenuItem* item = Gtk::manage(new Gtk::MenuItem("Add pass above", false));
	item->signal_activate().connect(sigc::mem_fun(*this, &MenuWidget::OnPassAddAbove));
	m_PassPopupMenu.append(*item);

	item = Gtk::manage(new Gtk::MenuItem("Add pass below", false));
	item->signal_activate().connect(sigc::mem_fun(*this, &MenuWidget::OnPassAddBelow));
	m_PassPopupMenu.append(*item);

	item = Gtk::manage(new Gtk::MenuItem("Remove pass", false));
	item->signal_activate().connect(sigc::mem_fun(*this, &MenuWidget::OnPassRemove));
	m_PassPopupMenu.append(*item);

	item = Gtk::manage(new Gtk::MenuItem("Add render target", false));
	item->signal_activate().connect(sigc::mem_fun(*this, &MenuWidget::OnRenderTargetAdd));
	m_PassPopupMenu.append(*item);

	item = Gtk::manage(new Gtk::MenuItem("Remove render target", false));
	item->signal_activate().connect(sigc::mem_fun(*this, &MenuWidget::OnRenderTargetRemove));
	m_PassPopupMenu.append(*item);

	m_PassPopupMenu.accelerate(m_PassTable);
	m_PassPopupMenu.show_all();

	// Add the notebook
	m_Container->add(m_WorkspaceNotebook);

	// Init dialog for adding passes
	m_PassAddDialog.set_modal();
	m_PassAddDialog.get_vbox()->pack_start(*Gtk::manage(new Gtk::Label("Pass name:")));
	m_PassAddDialog.get_vbox()->pack_end(m_PassAddName);
	m_PassAddDialog.add_button(GTK_STOCK_OK, Gtk::RESPONSE_OK);
	m_PassAddDialog.get_vbox()->show_all();
}

/**
 * Menu item click callback.
 * Adds a new page into the notebook, and marks it into the pages structure,
 * to not add it again on the next access of the current menu item.
 */
void MenuWidget::onMenuAccess(std::string name, std::string filename)
{
	int pageIndex;
	PageWidget* pageWidget;
	
	// If the page with the given name was already added, do not add it again.
	// Instead, set it as the current page.
	if (this->m_Pages[name] != NULL) {
		pageIndex = this->getPageIndexByName(name);
		// If the page cannot be found, stop the function execution.
		if (pageIndex == -1) {
			return;
		}

		this->m_Notebook->set_current_page(pageIndex);

		return;
	}

	// Build the page and attach it to the notebook.
	pageWidget = new PageWidget(this->m_Notebook, name, filename, &this->m_Pages);
	pageIndex = this->m_Notebook->append_page(*pageWidget, name);
	
	// Save the page.
	this->m_Pages[name] = pageWidget;

	// Show the notebook tabs and set the new page as the current one.
	this->m_Notebook->show_all();
	this->m_Notebook->set_current_page(pageIndex);
}

/**
 * Helper method used to fetch the page index, given its name.
 */
int MenuWidget::getPageIndexByName(std::string name)
{
	int i, nrPages;
	PageWidget* widget;
	
	nrPages = this->m_Notebook->get_n_pages();
	for (i = 0; i < nrPages; i++) {
		widget = (PageWidget*) this->m_Notebook->pages()[i].get_child();
		if (name == widget->getName()) {
			return i;
		}
	}

	return -1;
}

void MenuWidget::UpdatePassTable()
{
	m_arrPassRow.clear();
	m_PassTableTreeModel->clear();

	for (unsigned int i = 0, n = m_arrShaderPassDesc.size(); i < n; i++)
	{
		m_arrPassRow.push_back(*(m_PassTableTreeModel->append()));
		m_arrPassRow[i][m_TableColumn] = m_arrShaderPassDesc[i].name;
		Gtk::TreeModel::Row row = *(m_PassTableTreeModel->append(m_arrPassRow[i].children()));
		row[m_TableColumn] = "Vertex Shader - " + m_arrShaderPassDesc[i].name;
		row = *(m_PassTableTreeModel->append(m_arrPassRow[i].children()));
		row[m_TableColumn] = "Pixel Shader - " + m_arrShaderPassDesc[i].name;
		row = *(m_PassTableTreeModel->append(m_arrPassRow[i].children()));
		row[m_TableColumn] = "Object - " + m_arrShaderPassDesc[i].name;
	}

	m_PassTable.expand_all();
	m_PassTable.show_all();
}

void MenuWidget::UpdateConstantTable(unsigned int pass, std::vector<ShaderInputDesc>& constantTable)
{
	std::vector<ShaderPassDesc::ShaderConstants> backup(m_arrShaderPassDesc[pass].constants);
	m_arrShaderPassDesc[pass].constants.clear();

	for (unsigned int i = 0; i < constantTable.size(); i++)
	{
		switch (constantTable[i].eInputType)
		{
		case IT_BOOL:
		case IT_FLOAT:
		case IT_INT:
		{
			ShaderPassDesc::ShaderConstants constants;
			constants.cols = constantTable[i].nColumns;
			constants.inputType = constantTable[i].eInputType;
			constants.name = constantTable[i].szName;
			constants.rows = constantTable[i].nRows;
			constants.sizeOfArray = constantTable[i].nArrayElements;

			for (unsigned int j = 0; j < backup.size(); j++)
				if (constants.name == backup[j].name &&
					constants.cols == backup[j].cols &&
					constants.inputType == backup[j].inputType &&
					constants.rows == backup[j].rows &&
					constants.sizeOfArray == backup[j].sizeOfArray)
					for (unsigned int k = 0; k < 16; k++)
					{
						constants.valueFloat[k] = backup[j].valueFloat[k];
						break;
					}

			m_arrShaderPassDesc[pass].constants.push_back(constants);

			break;
		}
		case IT_SAMPLER:
		case IT_SAMPLER1D:
		case IT_SAMPLER2D:
		case IT_SAMPLER3D:
		case IT_SAMPLERCUBE:
		{
			ShaderPassDesc::ShaderConstants constants;
			constants.inputType = constantTable[i].eInputType;
			constants.name = constantTable[i].szName;

			for (unsigned int j = 0; j < backup.size(); j++)
				if (constants.name == backup[j].name &&
					constants.inputType == backup[j].inputType)
				{
					constants.sampler.boundTexture = backup[j].sampler.boundTexture;
					constants.sampler.samplerStates = backup[j].sampler.samplerStates;
					break;
				}

			m_arrShaderPassDesc[pass].constants.push_back(constants);

			break;
		}
		default:
			assert(false); // we don't support anything else
		}
	}

	m_arrConstantRow.clear();
	m_ConstantTableTreeModel->clear();

	for (unsigned int p = 0; p < m_arrShaderPassDesc.size(); p++)
	{
		m_arrConstantRow.push_back(*(m_ConstantTableTreeModel->append()));
		m_arrConstantRow[p][m_TableColumn] = m_arrShaderPassDesc[p].name;

		m_SamplersRow = *(m_ConstantTableTreeModel->append(m_arrConstantRow[p].children()));
		m_FloatsRow = *(m_ConstantTableTreeModel->append(m_arrConstantRow[p].children()));

		m_SamplersRow[m_TableColumn] = "Samplers";
		m_FloatsRow[m_TableColumn] = "Floats, Vectors, Matrices";

		for (unsigned int i = 0; i < m_arrShaderPassDesc[p].constants.size(); i++)
		{
			switch (m_arrShaderPassDesc[p].constants[i].inputType)
			{
			case IT_BOOL:
			case IT_FLOAT:
			case IT_INT:
			{
				Gtk::TreeModel::Row row = *(m_ConstantTableTreeModel->append(m_FloatsRow.children()));
				row[m_TableColumn] = m_arrShaderPassDesc[p].constants[i].name;
				break;
			}
			case IT_SAMPLER:
			case IT_SAMPLER1D:
			case IT_SAMPLER2D:
			case IT_SAMPLER3D:
			case IT_SAMPLERCUBE:
			{
				Gtk::TreeModel::Row row = *(m_ConstantTableTreeModel->append(m_SamplersRow.children()));
				row[m_TableColumn] = m_arrShaderPassDesc[p].constants[i].name;
				break;
			}
			default:
				assert(false); // we don't support anything else
			}
		}
	}

	m_ConstantTable.expand_all();
	m_ConstantTable.show_all();
}

void MenuWidget::OnPassItemActivated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* /* column */)
{
	Gtk::TreeModel::iterator iter = m_PassTableTreeModel->get_iter(path);
	if (iter)
	{
		Gtk::TreeModel::Row row = *iter;

		std::cout << "Row activated: Name=" << row->get_value(m_TableColumn) << std::endl;

		if (iter->parent()->get_stamp())
		{
			char str[10];
			strncpy_s(str, 10, row->get_value(m_TableColumn).c_str(), 9);
			if (strcmp(str, "Object - ") == 0)
			{
				Gtk::FileChooserDialog modelDialog("Choose model file");
				modelDialog.add_button("Select", Gtk::RESPONSE_OK);
				int ret = modelDialog.run();
				if (ret == Gtk::RESPONSE_OK)
				{
					Renderer* render = Renderer::GetInstance();
					ResourceManager* resMan = render->GetResourceManager();
					const unsigned int vbIdx = resMan->CreateVertexBuffer(modelDialog.get_filename().c_str());
					for (unsigned int p = 0; p < m_arrShaderPassDesc.size(); p++)
						if (m_arrShaderPassDesc[p].name == row->parent()->get_value(m_TableColumn))
						{
							m_arrShaderPassDesc[p].model = resMan->GetVertexBuffer(vbIdx);
							break;
						}
				}
			}
		}
		else
		{
			onMenuAccess(row->get_value(m_TableColumn), row->get_value(m_TableColumn));
		}
	}
}

void MenuWidget::OnConstantItemActivated(const Gtk::TreeModel::Path& path, Gtk::TreeViewColumn* /* column */)
{
	Gtk::TreeModel::iterator iter = m_ConstantTableTreeModel->get_iter(path);
	if (iter)
	{
		Gtk::TreeModel::Row row = *iter;

		if (!iter->parent()->get_stamp() || !iter->parent()->parent()->get_stamp())
			return; // Not good;

		std::cout << "Row activated: Name=" << row->get_value(m_TableColumn) << std::endl;

		for (unsigned int p = 0; p < m_arrShaderPassDesc.size(); p++)
		{
			if (m_arrShaderPassDesc[p].name == row->parent()->parent()->get_value(m_TableColumn))
			{
				for (unsigned int i = 0; i < m_arrShaderPassDesc[p].constants.size(); i++)
				{
					if (m_arrShaderPassDesc[p].constants[i].name == row->get_value(m_TableColumn))
					{
						switch (m_arrShaderPassDesc[p].constants[i].inputType)
						{
						case IT_BOOL:
						case IT_FLOAT:
						case IT_INT:
						{
							Gtk::Dialog matrixDialog;
							Gtk::VBox rowBox;
							Gtk::HBox colBox[4];
							Gtk::Entry entry[16];

							for (unsigned int j = 0; j < 16; j++)
							{
								char temp[32];
								sprintf_s(temp, "%f", m_arrShaderPassDesc[p].constants[i].valueFloat[j]);
								entry[j].set_text(temp);
							}

							unsigned int rows = m_arrShaderPassDesc[p].constants[i].rows;
							unsigned int cols = m_arrShaderPassDesc[p].constants[i].cols;
							unsigned int sizeOfArray = m_arrShaderPassDesc[p].constants[i].sizeOfArray;

							matrixDialog.get_vbox()->pack_start(rowBox);
							for (unsigned int r = 0; r < rows; r++)
							{
								rowBox.pack_start(colBox[r]);
								for (unsigned int c = 0; c < cols; c++)
								{
									colBox[r].pack_start(entry[r*cols + c]);
								}
							}

							matrixDialog.add_button(GTK_STOCK_OK, Gtk::RESPONSE_OK);
							matrixDialog.set_modal();
							matrixDialog.show_all();
							int ret = matrixDialog.run();
							if (ret == Gtk::RESPONSE_OK)
								for (unsigned int r = 0; r < rows; r++)
									for (unsigned int c = 0; c < cols; c++)
									{
										m_arrShaderPassDesc[p].constants[i].valueFloat[r*cols + c] = (float)atof(entry[r*cols + c].get_text().c_str());
									}

							break;
						}
						case IT_SAMPLER:
						case IT_SAMPLER1D:
						case IT_SAMPLER2D:
						case IT_SAMPLER3D:
						case IT_SAMPLERCUBE:
						{
							Gtk::FileChooserDialog textureDialog("Choose texture file");
							textureDialog.add_button("Select", Gtk::RESPONSE_OK);
							int ret = textureDialog.run();
							if (ret == Gtk::RESPONSE_OK)
							{
								Renderer* render = Renderer::GetInstance();
								ResourceManager* resMan = render->GetResourceManager();
								const unsigned int texIdx = resMan->CreateTexture(textureDialog.get_filename().c_str());
								m_arrShaderPassDesc[p].constants[i].sampler.boundTexture = resMan->GetTexture(texIdx);
							}

							break;
						}
						default:
							return;
						}

						return;
					}
				}

				return;
			}
		}
	}
}

void MenuWidget::OnPassWindowMenuPopup(GdkEventButton* ev)
{
	//return true;
	std::cout << "test1\n";
	
	if ((ev->type == GDK_BUTTON_PRESS) && (ev->button == 3))
	{
		m_PassPopupMenu.popup(ev->button, ev->time);
	}
}

void MenuWidget::OnPassAddAbove()
{
	Glib::RefPtr<Gtk::TreeView::Selection> selection = m_PassTable.get_selection();
	if (selection)
	{
		Gtk::TreeModel::iterator iter = selection->get_selected();
		if (iter)
		{
			std::cout << std::string((*iter)[m_TableColumn]) << std::endl;
			std::vector<ShaderPassDesc>::iterator it;
			for (it = m_arrShaderPassDesc.begin(); it < m_arrShaderPassDesc.end(); it++)
			{
				if (it->name == std::string((*iter)[m_TableColumn]))
					break;
			}
			if (it >= m_arrShaderPassDesc.end())
				return; // Not good...

			ShaderPassDesc newPass;
			m_PassAddName.set_text("");
			m_PassAddName.grab_focus();
			int ret = m_PassAddDialog.run();
			if (ret != Gtk::RESPONSE_OK)
				return;
			m_PassAddDialog.hide();
			newPass.name = m_PassAddName.get_text();
			m_arrShaderPassDesc.insert(it, newPass);
			UpdatePassTable();
		}
		else
		{
			// We don't have any passes yet, so let's add one
			ShaderPassDesc newPass;
			m_PassAddName.set_text("");
			m_PassAddName.grab_focus();
			int ret = m_PassAddDialog.run();
			if (ret != Gtk::RESPONSE_OK)
				return;
			m_PassAddDialog.hide();
			newPass.name = m_PassAddName.get_text();
			m_arrShaderPassDesc.push_back(newPass);
			UpdatePassTable();
		}
	}
}

void MenuWidget::OnPassAddBelow()
{
	Glib::RefPtr<Gtk::TreeView::Selection> selection = m_PassTable.get_selection();
	if (selection)
	{
		Gtk::TreeModel::iterator iter = selection->get_selected();
		if (iter)
		{
			std::cout << std::string((*iter)[m_TableColumn]) << std::endl;
			std::vector<ShaderPassDesc>::iterator it;
			for (it = m_arrShaderPassDesc.begin(); it < m_arrShaderPassDesc.end(); it++)
			{
				if (it->name == std::string((*iter)[m_TableColumn]))
					break;
			}
			if (it >= m_arrShaderPassDesc.end())
				return; // Not good...

			ShaderPassDesc newPass;
			m_PassAddName.set_text("");
			m_PassAddName.grab_focus();
			int ret = m_PassAddDialog.run();
			if (ret != Gtk::RESPONSE_OK)
				return;
			m_PassAddDialog.hide();
			newPass.name = m_PassAddName.get_text();
			m_arrShaderPassDesc.insert(it + 1, newPass);
			UpdatePassTable();
		}
		else
		{
			// We don't have any passes yet, so let's add one
			ShaderPassDesc newPass;
			m_PassAddName.set_text("");
			m_PassAddName.grab_focus();
			int ret = m_PassAddDialog.run();
			if (ret != Gtk::RESPONSE_OK)
				return;
			m_PassAddDialog.hide();
			newPass.name = m_PassAddName.get_text();
			m_arrShaderPassDesc.push_back(newPass);
			UpdatePassTable();
		}
	}
}

void MenuWidget::OnPassRemove()
{
	Glib::RefPtr<Gtk::TreeView::Selection> selection = m_PassTable.get_selection();
	if (selection)
	{
		Gtk::TreeModel::iterator iter = selection->get_selected();
		if (iter)
		{
			std::cout << std::string((*iter)[m_TableColumn]) << std::endl;
			std::vector<ShaderPassDesc>::iterator it;
			for (it = m_arrShaderPassDesc.begin(); it < m_arrShaderPassDesc.end(); it++)
			{
				if (it->name == std::string((*iter)[m_TableColumn]))
					break;
			}
			if (it >= m_arrShaderPassDesc.end())
				return; // Not good...

			m_arrShaderPassDesc.erase(it);
			UpdatePassTable();
		}
	}
}

void MenuWidget::OnRenderTargetAdd()
{
	std::cout << "IMPLEMENT ME!";
}

void MenuWidget::OnRenderTargetRemove()
{
	std::cout << "IMPLEMENT ME!";
}