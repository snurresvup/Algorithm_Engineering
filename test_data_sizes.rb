require 'open3'

runs = 10

def alg_to_s(alg)
  if alg == 0
    "BFS"
  elsif alg == 1
    "DFSL"
  elsif alg == 2
    "DFSR"
  end
end
=begin
(0..2).each do |alg|
  Dir["TraceTemplates/*"].each do |templ|
    puts "Using template: #{templ}"
    skew = 0.05
    tmpl = File.basename(templ, File.extname(templ))
    output = ""
    while skew<=0.95
      (1..runs).each do |i|
        #puts "executing - /Users/benner/Algorithm\\ Engineering/obj/binary_search_BST_DFSL.o 10000000 #{skew} 1"
        #system("/Users/benner/Algorithm\\ Engineering/obj/binary_search_BST_DFSl.o 10000000 #{skew} 1")
        #puts "executing - instruments -t #{templ} -D out/dfsl_skew_#{skew}_#{tmpl}_run_#{i}.trace obj/binary_search_BST_DFSL.o 10000000 #{skew} 1"
        system("instruments -t #{templ} -D out/#{alg_to_s alg}_#{skew}_#{tmpl}_#{i}.trace obj/binary_search_BST_DFSL.o 1000000 #{skew} #{alg}")
      end
      sum = 0
      (1..runs).each do |i|
        res, _ = Open3.capture2('./TraceUtil2', "out/#{alg_to_s alg}_#{skew}_#{tmpl}_#{i}.trace")
        sum += res.to_i
      end
      output += "#{skew} #{sum/runs}\n"
      skew = (skew + 0.05).round(2)
    end
    File.open("out/#{alg_to_s alg}_#{tmpl}_data", 'w') do |file| file.write(output) end
  end
end
=end
(0..2).each do |alg|
  n = 34000000
  output = ""
  while n <= 34000000
    puts "n=#{n}\n--CURRENT OUTPUT--\n#{output}\n"
    sum = 0
    (1..runs).each do |run|
      res, _ = Open3.capture2("./obj/binary_search_BST_DFSL.o #{n} 0.7 #{alg}")
      sum += res.to_i
    end
    output += "#{n} #{sum/runs}\n"
    n = n*2+1
  end
  File.open("out/#{alg_to_s alg}_running_time_0.7_data", 'w') do |file| file.write(output) end
end
