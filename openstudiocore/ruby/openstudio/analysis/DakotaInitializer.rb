########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
#  following disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
#  products derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
#  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
#  specific prior written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
#  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
#  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

require 'openstudio'
require "#{File.dirname(__FILE__)}/DakotaInterface.rb"
require "#{File.dirname(__FILE__)}/find_dakota.rb"

require 'pathname'

class DakotaParamsWatcher < OpenStudio::PathWatcher

  attr_accessor :dakotaInterface

  def initialize(p,msec,dakotaInterface)
    super(p,msec)
    @dakotaInterface = dakotaInterface
  end

  def onPathAdded
    puts "Executing onPathAdded."
    doFunctionEvaluation
  end

  def onPathChanged
    puts "Executing onPathChanged."
    doFunctionEvaluation
  end

  def doFunctionEvaluation
    @dakotaInterface.eval
    clearState
  end

end

class DakotaInitializer

  attr_accessor :problem, :out_dir, :prob_file, :restart_file, :dakota_exe
  attr_accessor :driver, :nodes, :cores, :pathWatcher, :dakotaInterface
  attr_accessor :full_out_dir, :full_restart_file
  
  def initialize(out_dir,prob_file,x0,
                 driver=true,nodes=1,cores=4,verbose=false,restart_file=OpenStudio::Path.new)
                 
    @out_dir = out_dir.to_s
    @prob_file = prob_file.to_s
    @restart_file = restart_file.to_s
    @problem = instantiate_problem(prob_file.to_s,x0,verbose)
    dakota_hash = OpenStudio::Analysis::find_dakota(5,3,1)
    @dakota_exe = dakota_hash[:dakota_exe]
    @driver = driver
    @nodes = nodes
    @cores = cores
    
    @full_out_dir = OpenStudio::system_complete(OpenStudio::Path.new(@out_dir))
    @full_restart_file = OpenStudio::system_complete(OpenStudio::Path.new(@restart_file)) if not @restart_file.empty?
    
    puts "Working directory is: " + Pathname.new(@out_dir).to_s + "."
    puts "Will use an explicit driver to receive data from Dakota." if driver
    puts "Will watch params.in path to receive data from Dakota." if not driver
    
  end
  
  def instantiate_problem(prob_file,x0,verbose)
    return nil if prob_file.empty?
    
    require prob_file.to_s
    
    ObjectSpace.each_object(OptimizationProblem) { |obj|
      return eval(obj.class.to_s + ".new(x0,verbose)")
    }
    
    raise prob_file.to_s + " does not appear to instantiate an instance of a class derived from OptimizationProblem."
  end
  
  def set_up_run()
    create_in_file()
    if @driver
      create_driver_file() 
    else
      create_path_watcher()
    end
  end
    
  def create_in_file()
  
    in_file_path = @full_out_dir.to_s + "/dakota.in"
    cur_dir = Dir.getwd
    
    puts "Current directory is: '" + cur_dir + "'"
    puts "Writing dakota.in to: '" + in_file_path + "'"
    
    outFile = File.open(in_file_path,"w")
    
    outFile.puts in_file_header()
    
    outFile.puts in_file_strategy()
    
    outFile.puts in_file_method()
    
    # outFile.puts in_file_model()
    
    outFile.puts in_file_variables()
    
    outFile.puts in_file_interface()
    
    outFile.puts in_file_responses()
      
    outFile.close
          
  end
  
  def in_file_header()
    result = String.new
    result << "# Autogenerated by DakotaInitializer.rb\n\n"
    return result
  end
  
  def in_file_strategy()
    result = String.new
    result << "strategy,\n"
    result << "        single_method\n"
    result << "        tabular_graphics_data\n"
    result << "        tabular_graphics_file='dakota_tabular_"
    @restart_file.empty? ? result << "1.dat'\n\n" : result << "2.dat'\n"
    return result  
  end
    
  def in_file_method()
    raise "Derived class must define."
  end
    
  def in_file_model()
    result = String.new
    result << "model,\n"
    result << "  single\n\n"
    return result
  end
    
  def in_file_variables()
    result = String.new
    result << "variables,\n"
    result << "        continuous_design = " + @problem.nx.to_s + "\n"
    result << "\n"
    return result
  end
    
  def in_file_interface()
    result = String.new
    if @driver
      result << "interface,\n"
      result << "        fork\n"
      result << "          asynchronous\n"
      result << "          evaluation_concurrency = " + @cores.to_s + "\n"
      result << "          analysis_driver = \"ruby -I'#{$OpenStudio_Dir}' dakota_driver.rb\"\n"
      result << "          parameters_file = 'params.in'\n"
      result << "          results_file    = 'results.out'\n"
      result << "            file_tag\n"
      result << "\n"
    else
      dummy_driver_path = @full_out_dir / OpenStudio::Path.new("dummy_driver.rb")
      params_in_path = @full_out_dir / OpenStudio::Path.new("params.in")
      results_out_path = @full_out_dir / OpenStudio::Path.new("results.out")
      
      result << "interface,\n"
      result << "        fork\n"
      result << "          analysis_driver = 'ruby " + dummy_driver_path.to_s + "'\n"
      result << "          parameters_file = '" + params_in_path.to_s + "'\n"
      result << "          results_file = '" + results_out_path.to_s + "'\n"
      result << "\n"
    end
    return result  
  end
    
  def in_file_responses()  
    result = String.new
    result << "responses,\n"
    result << "        num_objective_functions = 1\n"
    result << "        no_gradients\n"
    result << "        no_hessians\n\n"
    return result  
  end
  
  def create_driver_file()
    
    cur_dir = Dir.getwd
    
    puts "Current directory is: '" + cur_dir + "'"
    puts "Writing dakota_driver.rb to: '" + @out_dir + "/dakota_driver.rb" + "'"
    
    outFile = File.open(@out_dir + "/dakota_driver.rb","w")
    
    outFile.puts "# Autogenerated by DakotaInitializer.rb"
    outFile.puts ""
    outFile.puts "require 'openstudio'"
    outFile.puts "require '#{File.dirname(__FILE__)}/DakotaInterface.rb'"
    outFile.puts ""
    outFile.puts "di = DakotaInterface.new(\"" + @prob_file.to_s + "\",ARGV[0],ARGV[1])"
    outFile.puts "di.eval()"
    outFile.puts ""
    
    outFile.close
    
  end
  
  def create_path_watcher()
    puts "Creating DakotaInterface and dummy_driver.rb"
  
    params_in_path = @full_out_dir / OpenStudio::Path.new("params.in")
    results_out_path = @full_out_dir / OpenStudio::Path.new("results.out")
    @dakotaInterface = DakotaInterface.new(@prob_file,params_in_path.to_s,results_out_path.to_s)

    @pathWatcher = DakotaParamsWatcher.new(params_in_path,50,@dakotaInterface)
    
    outFile = File.open(@out_dir + "/dummy_driver.rb","w")
    outFile.puts "# Autogenerated by DakotaInitializer.rb"
    outFile.puts ""
    outFile.puts "puts 'Looking for ' + ARGV[1]"
    outFile.puts "cnt = 0"
    outFile.puts "while cnt < 40 and not File.exist?(ARGV[1])"
    outFile.puts "  sleep 0.25"
    outFile.puts "  cnt += 1"
    outFile.puts "end"
    outFile.puts "if cnt == 40"
    outFile.puts "  puts 'Unsuccessful.'"
    outFile.puts "end"
    outFile.close
  end
  
  def kick_off_run()
    cur_dir = Dir.getwd
    Dir.chdir(@out_dir)
    puts "Attempting to kick off run from directory " + @out_dir + "."
    if not (@dakota_exe == nil)
      dakotaCall = "dakota.in -o syscall"
      if @restart_file.empty? 
        dakotaCall << "1.out -write_restart dakota1.rst"
      else 
        dakotaCall << "2.out -write_restart dakota2.rst -read_restart '" << @full_restart_file.to_s << "'"
      end
      if driver
        if @nodes > 1
          puts "Calling mpirun to launch Dakota."
          system("mpirun -np " + @nodes.to_s + " " + @dakota_exe.to_s + " -i " + dakotaCall)
        else
          puts "Launching Dakota."
          system(@dakota_exe.to_s + " -i " + dakotaCall)
        end
      else 
        puts "Setting up RunManager RubyJob for kicking off Dakota."        
        
        outFile = File.open(@full_out_dir.to_s + "/runDakota.rb","w")
        outFile.puts "system(\"" + @dakota_exe.to_s + " -i ../" + dakotaCall + "\")"
        outFile.puts "puts 'Done.'"
        outFile.close
        workflow = OpenStudio::Runmanager::Workflow.new
        rubyJobBuilder = OpenStudio::Runmanager::RubyJobBuilder.new
        rubyJobBuilder.setScriptFile(@full_out_dir / OpenStudio::Path.new("runDakota.rb"))
        workflow.addJob(rubyJobBuilder.toWorkItem())
        workflow.add(OpenStudio::Runmanager::ConfigOptions::makeTools(
          OpenStudio::Path.new,
          OpenStudio::Path.new,
          OpenStudio::Path.new,
          $OpenStudio_RubyExeDir,
          OpenStudio::Path.new))
        job = workflow.create(@full_out_dir)
        rm_path = @full_out_dir / OpenStudio::Path.new("run.db")
        OpenStudio::remove(rm_path) if OpenStudio::exists(rm_path)
        runManager = OpenStudio::Runmanager::RunManager.new(OpenStudio::Path.new(rm_path))
        runManager.enqueue(job,false)
        running = true
        
        puts "Monitoring DAKOTA"
        while (running)
          OpenStudio::Application::instance.processEvents
          sleep 0.05
          # puts "Job tree status = " + job.treeStatus.valueName
          running = runManager.getJob(job.uuid).treeRunning
        end
        puts "Monitoring Complete"
      end
    else
      puts "Cannot kick off Dakota run because no dakota.exe was found."
    end
    Dir.chdir(cur_dir)
  end

end
