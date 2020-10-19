webview = {
	source = path.join(dependencies.basePath, "webview"),
}

function webview.import()
	links { "webview" }
	webview.includes()
end

function webview.includes()
	includedirs {
		webview.source,
		path.join(webview.source, "script"),
	}
end

function webview.project()
	project "webview"
		language "C++"

		webview.includes()

		files {
			path.join(webview.source, "webview.h"),
			path.join(webview.source, "webview.cc"),
		}

		warnings "Off"
		kind "StaticLib"
end

table.insert(dependencies, webview)
