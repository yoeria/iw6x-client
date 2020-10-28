onetbb = {
	source = path.join(dependencies.basePath, "oneTBB"),
}

function onetbb.import()
	links { "oneTBB" }
	onetbb.includes()
end

function onetbb.includes()
	includedirs {
		path.join(onetbb.source, "include"),
	}

	defines {
		"__TBB_NO_IMPLICIT_LINKAGE=1",
		"USE_WINTHREAD",
		"TBB_SUPPRESS_DEPRECATED_MESSAGES",
	}
end

function onetbb.project()
	project "oneTBB"
		language "C++"

		onetbb.includes()
		rapidjson.import();

		defines {
			"__TBB_BUILD",
			"__TBB_DYNAMIC_LOAD_ENABLED=0",
			"_TBB_USE_DEBUG=0",
			"_HAS_STD_BYTE=0"
		}

		includedirs {
			path.join(onetbb.source, "src"),
			path.join(onetbb.source, "src/rml/include"),
			path.join(onetbb.source, "build/vs2013"),
		}

		files {
			path.join(onetbb.source, "src/tbb/concurrent_hash_map.cpp"),
			path.join(onetbb.source, "src/tbb/concurrent_queue.cpp"),
			path.join(onetbb.source, "src/tbb/concurrent_vector.cpp"),
			path.join(onetbb.source, "src/tbb/dynamic_link.cpp"),
			path.join(onetbb.source, "src/tbb/itt_notify.cpp"),
			path.join(onetbb.source, "src/tbb/cache_aligned_allocator.cpp"),
			path.join(onetbb.source, "src/tbb/pipeline.cpp"),
			path.join(onetbb.source, "src/tbb/queuing_mutex.cpp"),
			path.join(onetbb.source, "src/tbb/queuing_rw_mutex.cpp"),
			path.join(onetbb.source, "src/tbb/reader_writer_lock.cpp"),
			path.join(onetbb.source, "src/tbb/spin_rw_mutex.cpp"),
			path.join(onetbb.source, "src/tbb/x86_rtm_rw_mutex.cpp"),
			path.join(onetbb.source, "src/tbb/spin_mutex.cpp"),
			path.join(onetbb.source, "src/tbb/critical_section.cpp"),
			path.join(onetbb.source, "src/tbb/mutex.cpp"),
			path.join(onetbb.source, "src/tbb/recursive_mutex.cpp"),
			path.join(onetbb.source, "src/tbb/condition_variable.cpp"),
			path.join(onetbb.source, "src/tbb/tbb_thread.cpp"),
			path.join(onetbb.source, "src/tbb/concurrent_monitor.cpp"),
			path.join(onetbb.source, "src/tbb/semaphore.cpp"),
			path.join(onetbb.source, "src/tbb/private_server.cpp"),
			path.join(onetbb.source, "src/rml/client/rml_tbb.cpp"),
			path.join(onetbb.source, "src/tbb/tbb_misc.cpp"),
			path.join(onetbb.source, "src/tbb/tbb_misc_ex.cpp"),
			path.join(onetbb.source, "src/tbb/task.cpp"),
			path.join(onetbb.source, "src/tbb/task_group_context.cpp"),
			path.join(onetbb.source, "src/tbb/governor.cpp"),
			path.join(onetbb.source, "src/tbb/market.cpp"),
			path.join(onetbb.source, "src/tbb/arena.cpp"),
			path.join(onetbb.source, "src/tbb/scheduler.cpp"),
			path.join(onetbb.source, "src/tbb/observer_proxy.cpp"),
			path.join(onetbb.source, "src/tbb/tbb_statistics.cpp"),
			path.join(onetbb.source, "src/tbb/tbb_main.cpp"),
			
			path.join(onetbb.source, "src/tbb/intel64-masm/*.asm"),
		}

		warnings "Off"
		kind "StaticLib"
end

table.insert(dependencies, onetbb)
