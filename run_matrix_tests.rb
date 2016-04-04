require 'open3'

runs = 5

Dir["obj/*.o"].each do |obj|
  prog = File.basename(obj, File.extname(obj))
  Dir["TraceTemplates/*"].each do |templ|
    tmpl = File.basename(templ, File.extname(templ))
    n = 31
    output = ""
    while n <= 5000 
      (1..runs).each do |run|
        system("instruments -t #{templ} -D out/#{prog}_#{tmpl}_#{n}_#{run}.trace #{obj} #{n}")
      end
      sum = 0
      (1..runs).each do |run|
        res, _ = Open3.capture2('./TraceUtil3', "out/#{prog}_#{tmpl}_#{n}_#{run}.trace")
        sum += res.to_i
      end
      output += "#{n} #{sum/runs}\n"
      n = n*2 + 1
    end
    File.open("data/#{prog}_#{tmpl}_data", 'w') do |file| file.write(output) end
  end
end
