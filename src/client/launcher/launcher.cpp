#include <std_include.hpp>
#include "launcher.hpp"
#include "utils/binary_resource.hpp"
#include "utils/nt.hpp"

namespace
{
	const utils::nt::module& get_webiew_2_loader()
	{
		static utils::binary_resource webview_2_loader(WEBVIEW_2_LOADER, "WebView2Loader.dll");
		static const auto loader = utils::nt::module::load(webview_2_loader.get_extracted_file());
		return loader;
	}

	struct lul
	{
		lul()
		{
			get_webiew_2_loader();
		}
	} _;
}

HRESULT WINAPI CreateCoreWebView2EnvironmentWithOptions(PCWSTR browserExecutableFolder, PCWSTR userDataFolder,
                                                        ICoreWebView2EnvironmentOptions* environmentOptions,
                                                        ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler*
                                                        environment_created_handler)
{
	return get_webiew_2_loader().invoke_pascal<HRESULT>("CreateCoreWebView2EnvironmentWithOptions",
	                                                    browserExecutableFolder,
	                                                    userDataFolder, environmentOptions,
	                                                    environment_created_handler);
}

std::string url_encode(const std::string& value)
{
	std::ostringstream escaped;
	escaped.fill('0');
	escaped << std::hex;

	for (auto i = value.begin(), n = value.end(); i != n; ++i)
	{
		const std::string::value_type c = (*i);

		// Keep alphanumeric and other accepted characters intact
		if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
		{
			escaped << c;
			continue;
		}

		// Any other characters are percent-encoded
		escaped << std::uppercase;
		escaped << '%' << std::setw(2) << int(static_cast<unsigned char>(c));
		escaped << std::nouppercase;
	}

	return escaped.str();
}

launcher::launcher()
{
	{
		webview::webview w(true, nullptr);
		w.set_title("IW6x");
		w.set_size(750, 420 - 38, WEBVIEW_HINT_FIXED);
		w.navigate("data:text/html," + url_encode(load_content(MENU_MAIN)));
		w.run();
	}

	this->create_main_menu();
}

void launcher::create_main_menu()
{
	this->main_window_.register_callback("openUrl", [](html_frame::callback_params* params)
	{
		if (params->arguments.empty()) return;

		const auto param = params->arguments[0];
		if (!param.is_string()) return;

		const auto url = param.get_string();
		ShellExecuteA(nullptr, "open", url.data(), nullptr, nullptr, SW_SHOWNORMAL);
	});

	this->main_window_.register_callback("selectMode", [this](html_frame::callback_params* params)
	{
		if (params->arguments.empty()) return;

		const auto param = params->arguments[0];
		if (!param.is_number()) return;

		const auto number = static_cast<mode>(param.get_number());
		if (number == mode::singleplayer || number == mode::multiplayer)
		{
			this->select_mode(static_cast<mode>(number));
		}
	});

	this->main_window_.set_callback(
		[](window* window, const UINT message, const WPARAM w_param, const LPARAM l_param) -> LRESULT
		{
			if (message == WM_CLOSE)
			{
				window::close_all();
			}

			return DefWindowProcA(*window, message, w_param, l_param);
		});

	this->main_window_.create("IW6x", 750, 420);
	this->main_window_.load_html(load_content(MENU_MAIN));
	this->main_window_.show();
}

launcher::mode launcher::run() const
{
	window::run();
	return this->mode_;
}

void launcher::select_mode(const mode mode)
{
	this->mode_ = mode;
	this->main_window_.close();
}

std::string launcher::load_content(const int res)
{
	return utils::nt::load_resource(res);
}
