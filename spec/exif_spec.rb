require 'exif'

describe Exif do
  before :all do
    @data = Exif::Data.new(File.expand_path('../sample.jpg', __FILE__))
  end

  # it '#dump' do
  #   expect { @data.dump }.not_to raise_error
  # end

  it 'works' do
    expect(@data.to_h[0][:image_width]).to eq 4000
    expect(@data.to_h[:image_width]).to eq 4000
    expect(@data[0][:image_width]).to eq 4000
    expect(@data[:image_width]).to eq 4000
    expect(@data.image_width).to eq 4000
    expect(@data.to_h[:gps][:gps_latitude]).to eq '24, 10.6817,  0'
    expect(@data.to_h[:gps_latitude]).to eq '24, 10.6817,  0'
    expect(@data[:gps][:gps_latitude]).to eq '24, 10.6817,  0'
    expect(@data[:gps_latitude]).to eq '24, 10.6817,  0'
    expect(@data.gps_latitude).to eq '24, 10.6817,  0'
  end
end